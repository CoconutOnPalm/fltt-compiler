#pragma once

#include <print>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Div : public TAC
	{
	private:

		const size_t left_index;
		const size_t right_index;
	
	public:

		Div(const size_t left, const size_t right, const std::string_view owning_proc)
			: TAC(owning_proc), left_index(left), right_index(right)
		{}

		virtual ~Div() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[left_index].useIn(p_index);
			info_table[right_index].useIn(p_index);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, const std::vector<TACInfo>& info_table) const override
		{
			//  auto label_align = std::make_shared<Asm::Label>("DIV_ALIGN");
			// auto label_calc  = std::make_shared<Asm::Label>("DIV_CALC");
			// auto label_check = std::make_shared<Asm::Label>("DIV_CHECK");
			// auto label_end   = std::make_shared<Asm::Label>("DIV_END");

			uint64_t div_align = tac::Label::nextUUID();
			uint64_t div_calc = tac::Label::nextUUID();
			uint64_t div_check = tac::Label::nextUUID();
			uint64_t div_end = tac::Label::nextUUID();
			uint64_t ret_zero = tac::Label::nextUUID();
			uint64_t div_exit = tac::Label::nextUUID();

			// RA = result
			// RB = right
			// RC = left
			// RD = quotent
			// RE = mask

			// check left == 0
			REG left = regalloc.getValue(left_index);
			left = regalloc.swap(left);
			asm_table.add<ins::JZERO>(ret_zero); // (0 / x) division
			regalloc.swap(REG::RC); // RC = left

			REG right = regalloc.getValue(right_index);
			right = regalloc.swap(right); // RA = right;
			regalloc.swap(REG::RB); // RB = right

			regalloc.resetRegister(REG::RA);
			regalloc.resetRegister(REG::RB);
			regalloc.resetRegister(REG::RC);
			regalloc.resetRegister(REG::RD);
			regalloc.resetRegister(REG::RE);

			// // Load Divisor (arg2) -> rb
			// load_to_ra(*instr.arg2); 
			// // CHECK ZERO BEFORE SWAP (while value is still in RA)
			// asm_code.push_back(Asm::make(Code::JZERO, label_end)); 
			// asm_code.push_back(Asm::make(Code::SWAP, Register::RB)); // rb = Divisor
			
			// check left < right;
			asm_table.add<ins::MOVE>(REG::RB); // RA = right
			asm_table.add<ins::JZERO>(ret_zero); // division by 0
			asm_table.add<ins::SUB>(REG::RC); // RA = left < right (JNZ right - left)
			asm_table.add<ins::JPOS>(ret_zero); // when jumping RA != 0

			// // Load Dividend (arg1) -> RC
			// load_to_ra(*instr.arg1);
			// asm_code.push_back(Asm::make(Code::SWAP, Register::RC)); // RC = Dividend (Remainder)

			// // Init Quotient (RD) = 0
			// asm_code.push_back(Asm::make(Code::RESET, Register::RA)); // ra = 0
			// asm_code.push_back(Asm::make(Code::SWAP, Register::RD));  // RD = 0, RA = OldRD

			asm_table.add<ins::RST>(REG::RD);
			
			// // Init Mask (RE) = 1
			// asm_code.push_back(Asm::make(Code::RESET, Register::RA)); // ra = 0
			// asm_code.push_back(Asm::make(Code::INC, Register::RA));   // ra = 1
			// asm_code.push_back(Asm::make(Code::SWAP, Register::RE));  // RE = 1, RA = OldRE

			asm_table.add<ins::RST>(REG::RE);
			asm_table.add<ins::INC>(REG::RE);

			// // Alignment Phase (Shift Divisor Left)
			// asm_code.push_back(Asm::make(Code::LABEL, label_align));

			asm_table.add<ins::LABEL>(div_align);

			// // Check: Is Divisor (RB) > Remainder (RC)?
			// // Calc: RA = rb - RC
			// asm_code.push_back(Asm::make(Code::RESET, Register::RA));
			// asm_code.push_back(Asm::make(Code::ADD, Register::RB));
			// asm_code.push_back(Asm::make(Code::SUB, Register::RC));

			asm_table.add<ins::MOVE>(REG::RB);
			asm_table.add<ins::SUB>(REG::RC);
			
			// // If rb - RC > 0, rb is too big, so we are done aligning.
			// asm_code.push_back(Asm::make(Code::JPOS, label_calc));

			asm_table.add<ins::JPOS>(div_calc);

			// // Shift Divisor (RB) Left
			// asm_code.push_back(Asm::make(Code::RESET, Register::RA));
			// asm_code.push_back(Asm::make(Code::ADD, Register::RB));
			// asm_code.push_back(Asm::make(Code::SHL, Register::RA));
			// asm_code.push_back(Asm::make(Code::SWAP, Register::RB));

			asm_table.add<ins::SHL>(REG::RB);

			// // Shift Mask (RE) Left
			// asm_code.push_back(Asm::make(Code::RESET, Register::RA));
			// asm_code.push_back(Asm::make(Code::ADD, Register::RE));
			// asm_code.push_back(Asm::make(Code::SHL, Register::RA));
			// asm_code.push_back(Asm::make(Code::SWAP, Register::RE));

			asm_table.add<ins::SHL>(REG::RE);

			// asm_code.push_back(Asm::make(Code::JUMP, label_align));

			asm_table.add<ins::JUMP>(div_align);

			// // Calculation (Subtract and Shift Right)
			// asm_code.push_back(Asm::make(Code::LABEL, label_calc));

			asm_table.add<ins::LABEL>(div_calc);

			// // Loop Condition: If Mask (RE) is 0, we are finished.
			// asm_code.push_back(Asm::make(Code::RESET, Register::RA));
			// asm_code.push_back(Asm::make(Code::ADD, Register::RE));
			// asm_code.push_back(Asm::make(Code::JZERO, label_end));

			asm_table.add<ins::MOVE>(REG::RE);
			asm_table.add<ins::JZERO>(div_end);

			// // Check: Does Divisor rb fit in Remainder RC?
			// // We want to know if RC >= rb.
			// // Test: RA = rb - RC.
			// // If RA > 0 (Pos), then rb > RC (Doesn't fit).
			// asm_code.push_back(Asm::make(Code::RESET, Register::RA));
			// asm_code.push_back(Asm::make(Code::ADD, Register::RB));
			// asm_code.push_back(Asm::make(Code::SUB, Register::RC));
			// asm_code.push_back(Asm::make(Code::JPOS, label_check));

			asm_table.add<ins::MOVE>(REG::RB);
			asm_table.add<ins::SUB>(REG::RC);
			asm_table.add<ins::JPOS>(div_check);

			// // IT FITS:
			// // Remainder = Remainder - Divisor
			// asm_code.push_back(Asm::make(Code::RESET, Register::RA));
			// asm_code.push_back(Asm::make(Code::ADD, Register::RC));
			// asm_code.push_back(Asm::make(Code::SUB, Register::RB));
			// asm_code.push_back(Asm::make(Code::SWAP, Register::RC));

			asm_table.add<ins::SWP>(REG::RC);
			asm_table.add<ins::SUB>(REG::RB);
			asm_table.add<ins::SWP>(REG::RC);

			// // Quotient = Quotient + Mask
			// asm_code.push_back(Asm::make(Code::RESET, Register::RA));
			// asm_code.push_back(Asm::make(Code::ADD, Register::RD));
			// asm_code.push_back(Asm::make(Code::ADD, Register::RE));
			// asm_code.push_back(Asm::make(Code::SWAP, Register::RD));

			asm_table.add<ins::SWP>(REG::RD);
			asm_table.add<ins::ADD>(REG::RE);
			asm_table.add<ins::SWP>(REG::RD);

			// asm_code.push_back(Asm::make(Code::LABEL, label_check));

			asm_table.add<ins::LABEL>(div_check);

			// // Shift Divisor (RB) Right
			// asm_code.push_back(Asm::make(Code::RESET, Register::RA));
			// asm_code.push_back(Asm::make(Code::ADD, Register::RB));
			// asm_code.push_back(Asm::make(Code::SHR, Register::RA));
			// asm_code.push_back(Asm::make(Code::SWAP, Register::RB));

			asm_table.add<ins::SHR>(REG::RB);

			// // Shift Mask (RE) Right
			// asm_code.push_back(Asm::make(Code::RESET, Register::RA));
			// asm_code.push_back(Asm::make(Code::ADD, Register::RE));
			// asm_code.push_back(Asm::make(Code::SHR, Register::RA));
			// asm_code.push_back(Asm::make(Code::SWAP, Register::RE));

			asm_table.add<ins::SHR>(REG::RE);

			// asm_code.push_back(Asm::make(Code::JUMP, label_calc));

			asm_table.add<ins::JUMP>(div_calc);

			// // Finalize
			// asm_code.push_back(Asm::make(Code::LABEL, label_end));

			asm_table.add<ins::LABEL>(div_end);
			
			// (result) RA = Quotient (RD)
			asm_table.add<ins::MOVE>(REG::RD);
			asm_table.add<ins::JUMP>(div_exit);
			
			asm_table.add<ins::LABEL>(ret_zero);
			asm_table.add<ins::RST>(REG::RA);

			asm_table.add<ins::LABEL>(div_exit);

			regalloc.overrideRA(p_index);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({}) / ({})", left_index, right_index);
		}
		
	};
	
} // namespace fl
