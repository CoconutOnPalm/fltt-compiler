#pragma once

#include <print>

#include "../../tac.hpp"

#include "../../../ASM/instructions/shl.hpp"
#include "../../../ASM/instructions/shr.hpp"
#include "../../../ASM/instructions/move.hpp"
#include "../../../ASM/instructions/sub.hpp"

#include "../../../ASM/reg_utils.hpp"



namespace fl::tac
{
	class Mod2 : public TAC
	{
	private:

		const size_t lvalue;
	
	public:

		Mod2(const size_t lval, const std::string_view owning_proc)
			: TAC(owning_proc), lvalue(lval)
		{}

		virtual ~Mod2() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[lvalue].useIn(p_index);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, const std::vector<TACInfo>& info_table) const override
		{	
			// RA = temp
			REG temp_reg = regalloc.allocTemporary(RegAlloc::temp_tac[0]);
			temp_reg = regalloc.swap(temp_reg);
			REG left_reg = regalloc.getValue(lvalue);
			
			asm_table.add<ins::MOVE>(left_reg);

			asm_table.add<ins::SHR>(temp_reg);
			asm_table.add<ins::SHL>(temp_reg);

			// SWP temp_reg, left_reg
			// RA = left_reg, RX = temp_reg
			temp_reg = left_reg; left_reg = regalloc.swap(left_reg);
			asm_table.add<ins::SUB>(temp_reg);
			regalloc.overrideRA(p_index);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({}) % 2", lvalue);
		}
		
	};
	
} // namespace fl
