#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../../utils/const.hpp"
#include "../../tac.hpp"

#include "../../../ASM/instructions/store.hpp"


namespace fl::tac
{
	
	class ProcEnter : public TAC
	{
	private:

	public:

		ProcEnter(const std::string_view owning_proc)
			: TAC(owning_proc)
		{}

		virtual ~ProcEnter() = default;

		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::LABEL, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override 
		{
			const size_t return_address = symbol_tables[p_owning_procedure]->get(config::return_variable_name).address();

			asm_table.add<ins::LABEL>(symbol_tables[p_owning_procedure]->UUID());
			regalloc.overrideRA(RegAlloc::Register{
				.tac = p_index,
				.data_type = RegAlloc::DataType::VARIABLE,
				.address = return_address,
			});

			asm_table.add<ins::STORE>(return_address);
		}

		virtual std::string __debug_string() const
		{
			return std::format("func");
		}
	};
	
} // namespace fl
