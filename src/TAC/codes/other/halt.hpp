#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../../utils/const.hpp"
#include "../../tac.hpp"

#include "../../../ASM/instructions/halt.hpp"


namespace fl::tac
{
	
	class Halt : public TAC
	{
	private:

	public:

		Halt()
			: TAC(config::prog_entry_name)
		{}

		virtual ~Halt() = default;

		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::LABEL, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override 
		{
			asm_table.add<ins::HALT>();
		}

		virtual std::string __debug_string() const
		{
			return std::format("halt");
		}
	};
	
} // namespace fl
