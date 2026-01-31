#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"

#include "../../../ASM/instructions/read.hpp"
#include "../../../ASM/instructions/store.hpp"
#include "../../../ASM/instructions/rstore.hpp"


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
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, std::vector<TACInfo>& info_table) const override 
		{
			// pointers
			if (info_table[lvalue].code_type == TACType::POINTER)
			{
				REG val_reg = regalloc.allocTemporary(p_index);
				val_reg = regalloc.swap(val_reg); // RA = temp
				REG addr_reg = regalloc.get(lvalue);
				// val_reg = regalloc.get(p_index);
				asm_table.add<ins::READ>();
				asm_table.add<ins::RSTORE>(addr_reg);
			}
			else // variables
			{
				std::string assoc_id = info_table[lvalue].associated_variable;
				const Symbol& symbol = symbol_tables[p_owning_procedure]->get(assoc_id);
				REG reg = regalloc.allocVariable(p_index, symbol.address());
				reg = regalloc.swap(reg);
				asm_table.add<ins::READ>();
				regalloc.storeVariable(reg, symbol.address(), p_index);
			}
		}

		virtual std::string __debug_string() const
		{
			return std::format("READ ({})", lvalue);
		}
	};
	
} // namespace fl
