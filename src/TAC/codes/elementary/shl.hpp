#pragma once

#include <print>

#include "../../tac.hpp"

#include "../../../ASM/instructions/shl.hpp"


namespace fl::tac
{
	// WARNING: use only on temporaries
	class Shl : public TAC
	{
	private:

		const size_t code;
	
	public:

		Shl(const size_t code, const std::string_view owning_proc)
			: TAC(owning_proc), code(code)
		{}

		virtual ~Shl() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{

		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, std::vector<TACInfo>& info_table) const override
		{	
			REG reg = regalloc.getValue(code);
			reg = regalloc.swap(reg); // we have to swap due to the behaviour of other TACs
			asm_table.add<ins::SHL>(reg);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({}) << 1", code);
		}
		
	};
	
} // namespace fl
