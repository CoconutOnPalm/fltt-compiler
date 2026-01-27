#pragma once

#include <print>

#include "../../tac.hpp"

#include "../../../ASM/instructions/dec.hpp"
#include "../../../ASM/instructions/store.hpp"


namespace fl::tac
{
	
	class Dec : public TAC
	{
	private:

		const std::string identifier;
	
	public:

		Dec(const std::string_view id, const std::string_view owning_proc)
			: TAC(owning_proc), identifier(id)
		{}

		virtual ~Dec() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{

		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, const std::vector<TACInfo>& info_table) const override
		{	
			const size_t address = symbol_tables[p_owning_procedure]->get(identifier).address();
			
			REG lval_reg = regalloc.loadVariable(p_index, address);
			asm_table.add<ins::DEC>(lval_reg);
			regalloc.storeVariable(lval_reg, address);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("'{}'--", identifier);
		}
		
	};
	
} // namespace fl
