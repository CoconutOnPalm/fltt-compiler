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
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			size_t address = symbol_tables[p_owning_procedure]->get(identifier).address();
			regalloc.load(p_index, address);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("LD '{}'", identifier);
		}
	};
	
} // namespace fl
