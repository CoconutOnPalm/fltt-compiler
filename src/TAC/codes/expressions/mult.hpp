#pragma once

#include <print>

#include "../../tac.hpp"

#include "../../../ASM/instructions/shr.hpp"
#include "../../../ASM/instructions/shl.hpp"
#include "../../../ASM/instructions/move.hpp"
#include "../../../ASM/instructions/jump.hpp"
#include "../../../ASM/instructions/jzero.hpp"
#include "../../../ASM/instructions/jpos.hpp"
#include "../../../ASM/instructions/swp.hpp"
#include "../../../ASM/instructions/inc.hpp"
#include "../../../ASM/instructions/dec.hpp"
#include "../../../ASM/instructions/add.hpp"
#include "../../../ASM/instructions/sub.hpp"


namespace fl::tac
{
	
	class Mult : public TAC
	{
	private:

		const size_t left_index;
		const size_t right_index;
	
	public:

		Mult(const size_t left, const size_t right, const std::string_view owning_proc)
			: TAC(owning_proc), left_index(left), right_index(right)
		{}

		virtual ~Mult() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[left_index].useIn(p_index);
			info_table[right_index].useIn(p_index);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			// for this, use hard-coded registers
			// RA = total
			// RB = left
			// RC = right
			// RD = temp
			// RE = i;
			// RF = k;

			// i, temp, total = (0, 0, 0);
			// while (right > 0)
			// {
			//		temp = right
			// 		SHR right; SHL right
			//		if (right >= temp) // cheaper if (right == temp) <=> (right & 1) == 0
			//			i++;
			//			continue;
			//		temp = a;
			//		k = i;
			//		while (k > 0)
			//			SHL temp;
			//			k--;
			//		total += temp;
			// 		SHR right;
			// 		i++;
			// }

			uint64_t loop_begin = tac::Label::nextUUID();
			uint64_t loop_end = tac::Label::nextUUID();
			// uint64_t if_begin = tac::Label::nextUUID();
			uint64_t if_end = tac::Label::nextUUID();
			uint64_t pow_loop_begin = tac::Label::nextUUID();
			uint64_t pow_loop_end = tac::Label::nextUUID();

			REG reg_a = regalloc.getValue(left_index);
			reg_a = regalloc.swap(reg_a);
			regalloc.swap(REG::RB);
			REG reg_b = regalloc.getValue(right_index);
			reg_b = regalloc.swap(reg_b);
			regalloc.swap(REG::RC);
			
			asm_table.add<ins::RST>(REG::RA); // total = 0;
			asm_table.add<ins::RST>(REG::RD); // temp = 0;
			asm_table.add<ins::RST>(REG::RE); // i = 0;
			asm_table.add<ins::RST>(REG::RF); // k = 0;

			asm_table.add<ins::LABEL>(loop_begin); // while begin
			// JZ right
			asm_table.add<ins::SWP>(REG::RC); // RA = right, RC = total
			asm_table.add<ins::JZERO>(loop_end);

			// temp = right
			asm_table.add<ins::SWP>(REG::RD); // RA = temp, RC = total, RD = right
			asm_table.add<ins::MOVE>(REG::RD); // RA (temp) = right

			// SHR right; SHL right;
			asm_table.add<ins::SHR>(REG::RD); // SHR right
			asm_table.add<ins::SHL>(REG::RD); // SHL right

			// if (temp > right)
			asm_table.add<ins::SUB>(REG::RD);
			asm_table.add<ins::JPOS>(if_end);

			asm_table.add<ins::SWP>(REG::RD); // RA = right, RC = total, RD = temp
			asm_table.add<ins::SWP>(REG::RC); // RA = total, RC = right, RD = temp
			asm_table.add<ins::SHR>(REG::RC); // shr right
			asm_table.add<ins::INC>(REG::RE); // i++;
			asm_table.add<ins::JUMP>(loop_begin); // continue
			
			asm_table.add<ins::LABEL>(if_end);

			asm_table.add<ins::MOVE>(REG::RB); // temp = a

			asm_table.add<ins::SWP>(REG::RD); // RA = right, RC = total, RD = temp
			asm_table.add<ins::SWP>(REG::RC); // RA = total, RC = right, RD = temp

			asm_table.add<ins::SWP>(REG::RF); // RA = k, RF = total
			asm_table.add<ins::MOVE>(REG::RE); // k = i;

			// while (k <= 0)
			asm_table.add<ins::LABEL>(pow_loop_begin);
			asm_table.add<ins::JZERO>(pow_loop_end);
			
			asm_table.add<ins::SHL>(REG::RD); // SHL temp
			asm_table.add<ins::DEC>(REG::RA); // k--

			asm_table.add<ins::JUMP>(pow_loop_begin);
			asm_table.add<ins::LABEL>(pow_loop_end);

			asm_table.add<ins::SWP>(REG::RF); // RA = total, RF = k
			asm_table.add<ins::ADD>(REG::RD); // total += temp
			asm_table.add<ins::SHR>(REG::RC); // shr right
			asm_table.add<ins::INC>(REG::RE); // i++;

			asm_table.add<ins::JUMP>(loop_begin);

			asm_table.add<ins::LABEL>(loop_end);
			asm_table.add<ins::SWP>(REG::RC); // RA = total, RC = right

			regalloc.overrideRA(RegAlloc::Register{
				.tac = p_index,
				.data_type = RegAlloc::DataType::TEMPORARY,
				.address = 0,
			});
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({}) * ({})", left_index, right_index);
		}
		
	};
	
} // namespace fl
