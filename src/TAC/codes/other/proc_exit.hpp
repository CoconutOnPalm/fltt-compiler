#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../../utils/const.hpp"
#include "../../tac.hpp"

#include "../../../ASM/instructions/rtrn.hpp"


namespace fl::tac
{
	
	class ProcExit : public TAC
	{
	private:

	public:

		ProcExit(const std::string_view owning_proc)
			: TAC(owning_proc)
		{}

		virtual ~ProcExit() = default;

		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::LABEL, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override 
		{
			const size_t return_address = symbol_tables[p_owning_procedure]->get(config::return_variable_name).address();
			const REG ret = regalloc.loadVariable(p_index, return_address);
			regalloc.swap(ret);
			asm_table.add<ins::RTRN>();
		}

		virtual std::string __debug_string() const
		{
			return std::format("func");
		}
	};
	
} // namespace fl
