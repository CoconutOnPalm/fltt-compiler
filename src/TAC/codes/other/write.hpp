#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"

#include "../../../ASM/instructions/write.hpp"


namespace fl::tac
{
	
	class Write : public TAC
	{
	private:

		const size_t rvalue;

	public:

		Write(const size_t rval, const std::string_view owning_proc)
			: TAC(owning_proc), rvalue(rval)
		{}

		virtual ~Write() = default;

		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::IO, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[rvalue].useIn(p_index);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override 
		{
			regalloc.swap(regalloc.get(rvalue));
			asm_table.add<ins::WRITE>();
		}

		virtual std::string __debug_string() const
		{
			return std::format("WRITE ({})", rvalue);
		}
	};
	
} // namespace fl
