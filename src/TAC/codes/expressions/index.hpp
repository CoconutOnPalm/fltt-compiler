#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"

#include "../../../ASM/instructions/rload.hpp"
#include "../../../ASM/instructions/swp.hpp"
#include "../../../ASM/instructions/move.hpp"

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
		{
		}

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

				const size_t ptr_tac = RegAlloc::temp_tac[0];
				const size_t shift_tac = RegAlloc::temp_tac[1];

				REG ptr_reg = regalloc.loadVariable(ptr_tac, symbol_tables[p_owning_procedure]->get(identifier).address());
				REG shift_reg = regalloc.loadPointer(shift_tac, ptr_reg);
				ptr_reg = regalloc.loadVariable(ptr_tac, symbol_tables[p_owning_procedure]->get(identifier).address());

				// // load ptr_reg again and set it to the first elem
				// REG ptr_reg = regalloc.loadVariable(RegAlloc::temp_tac[0], symbol_tables[p_owning_procedure]->get(identifier).address());
				// asm_table.add<ins::INC>(ptr_reg); // set to the first elem

				// convert index register to the 'normal' 0..n array indexing
				REG index_reg = regalloc.getValue(index);
				index_reg = regalloc.swap(index_reg); // RA = index
				shift_reg = regalloc.get(shift_tac);
				asm_table.add<ins::SUB>(shift_reg);

				ptr_reg = regalloc.getValue(ptr_tac);
				asm_table.add<ins::INC>(ptr_reg); // set to the first elem

				// RA = index_reg
				asm_table.add<ins::ADD>(ptr_reg);
				// RA = &arr[i]
				regalloc.overrideRA(RegAlloc::Register{
					.tac = p_index,
					.data_type = RegAlloc::DataType::POINTER,
					.address = 0,
				});
				return;
			}


			Array* symbol = dynamic_cast<Array*>(&symbol_tables[p_owning_procedure]->get(identifier));
			if (symbol == nullptr)
			{
				panic("internal compiler error: could not find array index operator [] lvalue in symbol table, name={}",
					symbol_tables[p_owning_procedure]->get(identifier).name);
			}

			// RA = nullptr
			REG pointer_reg = regalloc.allocPointer(p_index);
			pointer_reg = regalloc.swap(pointer_reg);
			asm_table.add<ins::RST>(REG::RA);

			const REG index_reg = regalloc.getValue(index);

			// RA = index
			asm_table.add<ins::ADD>(index_reg);
			// asm_table.add<ins::MOVE>(index_reg);

			// forget index register and make it act as temporary
			size_t shift = symbol->begin; // we know the shift and compile time so we omit it
			std::println("[debug]: {}[({})] shift = {} + {} = {}", identifier, index, symbol->begin, symbol->header_size, shift);
			if (shift > symbol->address())
			{
				setupImmediate(shift - symbol->address() - symbol->header_size, index_reg, asm_table);
				asm_table.add<ins::SUB>(index_reg);
			}
			else
			{
				setupImmediate(symbol->address() - shift + symbol->header_size, index_reg, asm_table);
				asm_table.add<ins::ADD>(index_reg);
			}

			// RA = &arr[i]
		}

		virtual std::string __debug_string() const override
		{
			return std::format("{}[({})]", identifier, index);
		}
	};

} // namespace fl
