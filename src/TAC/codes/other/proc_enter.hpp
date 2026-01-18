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
		{
		}

		virtual ~ProcEnter() = default;

		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::LABEL, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
		}

		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			std::shared_ptr<SymbolTable> symbol_table = symbol_tables[p_owning_procedure];
			
			// store return address
			if (p_owning_procedure != config::prog_entry_name)
			{
				const size_t return_address = symbol_table->get(config::return_variable_name).address();

				asm_table.add<ins::LABEL>(symbol_tables[p_owning_procedure]->UUID());
				regalloc.overrideRA(RegAlloc::Register{
					.tac = RegAlloc::temp_tac[0],
					.data_type = RegAlloc::DataType::VARIABLE,
					.address = return_address,
					});

				asm_table.add<ins::STORE>(return_address);
			}

			// setup array headers
			for (auto& [id, symbol] : symbol_table->elements())
			{
				// only arrays declared in the func
				if (symbol->testFlag(SymbolType::ARRAY) && !symbol->testFlag(SymbolType::ARGUMENT))
				{
					regalloc.flushTemporaryTAC();
					Array* arr = dynamic_cast<Array*>(symbol.get());
					if (arr == nullptr)
						panic("array symbol '{}' is not actually an array", symbol->name);

					REG reg = regalloc.loadImmediate(RegAlloc::temp_tac[0], arr->begin);
					reg = regalloc.swap(reg);
					asm_table.add<ins::STORE>(arr->address() + 0); // first elem stores shift
				}
			}
		}

		virtual std::string __debug_string() const
		{
			return std::format("func");
		}
	};

} // namespace fl
