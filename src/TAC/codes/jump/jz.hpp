#pragma once


#include <memory>

#include "../../tac.hpp"
#include "../other/label.hpp"

#include "../../../ASM/instructions/jzero.hpp"


namespace fl::tac
{

	class JZ : public TAC
	{
	private:

		const size_t condition_index;
		std::shared_ptr<size_t> label_id;
		
	public:

		JZ(const size_t cond_index, std::shared_ptr<size_t> label, const std::string_view owning_proc)
			: TAC(owning_proc), condition_index(cond_index), label_id(label)
		{}
		virtual ~JZ() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::JUMP, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[condition_index].useIn(p_index);
		}

		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, std::vector<TACInfo>& info_table) const override
		{
			REG cond_reg = regalloc.get(condition_index);
			cond_reg = regalloc.swap(cond_reg);	// cond_reg should be RA, but swap jic

			asm_table.add<ins::JZERO>(*label_id);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("JZ ({}) --> {}", condition_index, *(label_id.get()));
		}
	};
	
} // namespace fl::tac
