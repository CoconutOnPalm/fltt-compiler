#pragma once

#include <print>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Mod : public TAC
	{
	private:

		const size_t left_index;
		const size_t right_index;
	
	public:

		Mod(const size_t left, const size_t right, const std::string_view owning_proc)
			: TAC(owning_proc), left_index(left), right_index(right)
		{}

		virtual ~Mod() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[left_index].useIn(p_index);
			info_table[right_index].useIn(p_index);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, std::vector<TACInfo>& info_table) const override
		{
			// division code yoinked from somewhere
			
			uint64_t div_align = tac::Label::nextUUID();
			uint64_t div_calc = tac::Label::nextUUID();
			uint64_t div_check = tac::Label::nextUUID();
			uint64_t div_end = tac::Label::nextUUID();
			uint64_t ret_zero = tac::Label::nextUUID();
			uint64_t div_exit = tac::Label::nextUUID();

			// RA = result
			// RB = right
			// RC = left
			// RD = quotient
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

			// check left < right;
			asm_table.add<ins::MOVE>(REG::RB); // RA = right
			asm_table.add<ins::JZERO>(ret_zero); // division by 0
			asm_table.add<ins::SUB>(REG::RC); // RA = left < right (JNZ right - left)
			asm_table.add<ins::JPOS>(div_end); // when jumping RA != 0

			asm_table.add<ins::RST>(REG::RD);
			
			// mask (RE) = 1
			asm_table.add<ins::RST>(REG::RE);
			asm_table.add<ins::INC>(REG::RE);

			// align
			asm_table.add<ins::LABEL>(div_align);

			asm_table.add<ins::MOVE>(REG::RB);
			asm_table.add<ins::SUB>(REG::RC);
			
			asm_table.add<ins::JPOS>(div_calc);

			// shift divisor (RB) <<
			asm_table.add<ins::SHL>(REG::RB);

			// shift mask (RE) <<
			asm_table.add<ins::SHL>(REG::RE);

			asm_table.add<ins::JUMP>(div_align);
			asm_table.add<ins::LABEL>(div_calc);

			// mask (RE) == 0 --> exit
			asm_table.add<ins::MOVE>(REG::RE);
			asm_table.add<ins::JZERO>(div_end);

			// check if divisor (RB) fits in remainder (RC)
			asm_table.add<ins::MOVE>(REG::RB);
			asm_table.add<ins::SUB>(REG::RC);
			asm_table.add<ins::JPOS>(div_check);

			// // Remainder = Remainder - Divisor
			// remainder (RC) -= divisor
			asm_table.add<ins::SWP>(REG::RC);
			asm_table.add<ins::SUB>(REG::RB);
			asm_table.add<ins::SWP>(REG::RC);

			// quotient (RD) += mask (RE)
			asm_table.add<ins::SWP>(REG::RD);
			asm_table.add<ins::ADD>(REG::RE);
			asm_table.add<ins::SWP>(REG::RD);

			asm_table.add<ins::LABEL>(div_check);

			// shift divisor (RB) >>
			asm_table.add<ins::SHR>(REG::RB);

			// shift mask (RE) >>
			asm_table.add<ins::SHR>(REG::RE);

			asm_table.add<ins::JUMP>(div_calc);

			// div end
			asm_table.add<ins::LABEL>(div_end);
			
			// (result) RA = Remainder (RC)
			asm_table.add<ins::MOVE>(REG::RC);
			asm_table.add<ins::JUMP>(div_exit);
			
			asm_table.add<ins::LABEL>(ret_zero);
			asm_table.add<ins::RST>(REG::RA);

			asm_table.add<ins::LABEL>(div_exit);

			// flush registers
			regalloc.resetRegister(REG::RA);
			regalloc.resetRegister(REG::RB);
			regalloc.resetRegister(REG::RC);
			regalloc.resetRegister(REG::RD);
			regalloc.resetRegister(REG::RE);

			// move result to RB (so it doesn't clog RA)
			asm_table.add<ins::SWP>(REG::RB);
			regalloc.overrideRegister(REG::RB, RegAlloc::Register{
				.tac = p_index,
				.data_type = RegAlloc::DataType::TEMPORARY,
				.address = 0,
			});

			// regalloc.overrideRA(p_index);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({}) % ({})", left_index, right_index);
		}
		
	};
	
} // namespace fl
