#pragma once

#include <print>

#include "../../tac.hpp"

#include "../../../ASM/instructions/add.hpp"


namespace fl::tac
{
	
	class Add : public TAC
	{
	private:

		const size_t left_index;
		const size_t right_index;
	
	public:

		Add(const size_t left, const size_t right, const std::string_view owning_proc)
			: TAC(owning_proc), left_index(left), right_index(right)
		{}

		virtual ~Add() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[left_index].useIn(p_index);
			info_table[right_index].useIn(p_index);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, const std::vector<TACInfo>& info_table) const override
		{
			// RA = left
			regalloc.swap(regalloc.getValue(left_index));
			// ADD RX
			REG rreg = regalloc.getValue(right_index);
			asm_table.add<ins::ADD>(rreg);
			// RA = result
			regalloc.overrideRA(p_index);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({}) + ({})", left_index, right_index);
		}
	};
	
} // namespace fl
