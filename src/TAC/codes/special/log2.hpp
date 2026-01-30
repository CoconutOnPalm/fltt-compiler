#pragma once

#include <print>

#include "../../tac.hpp"

#include "../../../ASM/instructions/shr.hpp"
#include "../../../ASM/instructions/move.hpp"

#include "../../../ASM/reg_utils.hpp"



namespace fl::tac
{
	class Log2 : public TAC
	{
	private:

		const size_t lvalue;
		const uint64_t exponent; // power of 2
	
	public:

		Log2(const size_t lval, const uint64_t exp, const std::string_view owning_proc)
			: TAC(owning_proc), lvalue(lval), exponent(exp)
		{}

		virtual ~Log2() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[lvalue].useIn(p_index);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, std::vector<TACInfo>& info_table) const override
		{	
			// RA = output
			REG reg = regalloc.allocTemporary(p_index);
			reg = regalloc.swap(reg);
			REG left_reg = regalloc.getValue(lvalue);
			
			// copy to preserve left's register
			asm_table.add<ins::MOVE>(left_reg);

			for (size_t i = 0; i < exponent; i++)
				asm_table.add<ins::SHR>(reg);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({})^{}", lvalue, exponent);
		}
		
	};
	
} // namespace fl
