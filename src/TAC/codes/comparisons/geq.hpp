#pragma once

#include <print>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class GreaterOrEqual : public TAC
	{
	private:

		const size_t left {0};
		const size_t right {0};

	public:

		GreaterOrEqual(const size_t l, const size_t r, const std::string_view owning_proc)
			: TAC(owning_proc), left(l), right(r)
		{}

		virtual ~GreaterOrEqual() = default;

		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::CONDITION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[left].useIn(p_index);
			info_table[right].useIn(p_index);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override 
		{
			// RA = r, RX = l
			REG right_reg = regalloc.getValue(right);
			right_reg = regalloc.swap(right_reg);
			REG left_reg = regalloc.getValue(left);

			// RA = (r - l)
			asm_table.add<ins::SUB>(left_reg);
			regalloc.overrideRA(p_index);
		}

		virtual std::string __debug_string() const
		{
			return std::format("({}) > ({})", left, right);
		}
	};
	
} // namespace fl
