#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"

#include "../../../ASM/instructions/call.hpp"


namespace fl::tac
{
	
	class Call : public TAC
	{
	private:

		const std::string callee;

	public:

		Call(const std::string_view callee, const std::string_view owning_proc)
			: TAC(owning_proc), callee(callee)
		{}

		virtual ~Call() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::JUMP, p_owning_procedure);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, std::vector<TACInfo>& info_table) const override 
		{
			asm_table.add<ins::CALL>(symbol_tables[callee]->UUID());
		}

		virtual std::string __debug_string() const
		{
			return std::format("call @{}", callee);
		}
	};
	
} // namespace fl
