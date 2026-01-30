#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class LD : public TAC
	{

		std::string identifier;

	public:

		LD(const std::string_view id, const std::string_view owning_proc)
			: TAC(owning_proc), identifier(id)
		{}

		virtual ~LD() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::VARIABLE, p_owning_procedure, identifier);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, std::vector<TACInfo>& info_table) const override
		{
			const Symbol& symbol = symbol_tables[p_owning_procedure]->get(identifier);

			// pointers
			if (symbol.testFlag(SymbolType::ARGUMENT) || symbol.testFlag(SymbolType::ARRAY))
			{
				const REG address = regalloc.loadImmediate(p_index, symbol.address());
				regalloc.loadPointer(p_index, address);

				// correct info table
				info_table[p_index].code_type = TACType::POINTER;
			}
			else // variables
			{
				regalloc.loadVariable(p_index, symbol.address());
			}
		}

		virtual std::string __debug_string() const override
		{
			return std::format("LD '{}'", identifier);
		}
	};
	
} // namespace fl
