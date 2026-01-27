#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class LDC : public TAC
	{

		std::string identifier;

	public:

		LDC(const std::string_view id, const std::string_view owning_proc)
			: TAC(owning_proc), identifier(id)
		{}

		virtual ~LDC() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::CONSTANT, p_owning_procedure, identifier);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, const std::vector<TACInfo>& info_table) const override
		{
			const Symbol& symbol = symbol_tables[p_owning_procedure]->get(identifier);
			const REG reg = regalloc.loadVariable(p_index, symbol.address());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("LDC '{}'", identifier);
		}
	};
	
} // namespace fl
