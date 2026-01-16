#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"

#include "../../../ASM/instructions/rload.hpp"
#include "../../../ASM/instructions/swp.hpp"

#include "../../../ASM/reg_utils.hpp"

namespace fl::tac
{
	
	class Index : public TAC
	{

		std::string identifier;
		size_t index;

	public:

		Index(const std::string_view id, const size_t ind, const std::string_view owning_proc)
			: TAC(owning_proc), identifier(id), index(ind)
		{}

		virtual ~Index() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::ARRELEM, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[index].useIn(p_index);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			
			if (symbol_tables[p_owning_procedure]->get(identifier).testFlag(SymbolType::ARGUMENT))
			{
				// symbol = dynamic_cast<Array*>(&symbol_tables[p_owning_procedure]);
				Argument* arg = dynamic_cast<Argument*>(&symbol_tables[p_owning_procedure]->get(identifier));
				
			}
			
			
			Array* symbol = dynamic_cast<Array*>(&symbol_tables[p_owning_procedure]->get(identifier));
			if (symbol == nullptr)
			{
				panic("internal compiler error: could not find array index operator [] lvalue in symbol table, name={}", 
					symbol_tables[p_owning_procedure]->get(identifier).name);
			}

			// RA = nullptr
			REG pointer_reg = regalloc.allocPointer(p_index);
			regalloc.swap(pointer_reg);
			pointer_reg = REG::RA;
			asm_table.add<ins::RST>(REG::RA);

			const REG index_reg = regalloc.getValue(index);

			// RA = index
			asm_table.add<ins::ADD>(index_reg);
			
			// forget index register and make it act as temporary
			size_t shift = symbol->begin;
			if (shift > symbol->address())
			{
				setupImmediate(shift - symbol->address(), index_reg, asm_table);
				asm_table.add<ins::SUB>(index_reg);
			}
			else
			{
				setupImmediate(symbol->address() - shift, index_reg, asm_table);
				asm_table.add<ins::ADD>(index_reg);
			}

			// RA = array
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({})[({})]", identifier, index);
		}
	};
	
} // namespace fl
