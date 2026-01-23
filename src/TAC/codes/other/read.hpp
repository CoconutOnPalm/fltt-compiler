#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"

#include "../../../ASM/instructions/read.hpp"


namespace fl::tac
{
	
	class Read : public TAC
	{
	private:

		const size_t lvalue;

	public:

		Read(const size_t lval, const std::string_view owning_proc)
			: TAC(owning_proc), lvalue(lval)
		{}

		virtual ~Read() = default;

		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::IO, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[lvalue].useIn(p_index);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override 
		{
			regalloc.swap(regalloc.getValue(lvalue));
			asm_table.add<ins::READ>();
			regalloc.copy(lvalue, lvalue, lvalue);
		}

		virtual std::string __debug_string() const
		{
			return std::format("READ ({})", lvalue);
		}
	};
	
} // namespace fl
