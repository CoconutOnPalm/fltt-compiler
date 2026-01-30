#pragma once

#include <print>

#include "../../tac.hpp"

#include "../../../ASM/instructions/sub.hpp"

namespace fl::tac
{
	
	class Sub : public TAC
	{
	private:

		const size_t left_index;
		const size_t right_index;
	
	public:

		Sub(const size_t left, const size_t right, const std::string_view owning_proc)
			: TAC(owning_proc), left_index(left), right_index(right)
		{}

		virtual ~Sub() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[left_index].useIn(p_index);
			info_table[right_index].useIn(p_index);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, std::vector<TACInfo>& info_table) const override
		{
			// RA = left
			regalloc.swap(regalloc.getValue(left_index));
			// ADD RX
			asm_table.add<ins::SUB>(regalloc.getValue(right_index));
			// RA = result
			regalloc.overrideRA(p_index);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({}) - ({})", left_index, right_index);
		}
		
	};
	
} // namespace fl
