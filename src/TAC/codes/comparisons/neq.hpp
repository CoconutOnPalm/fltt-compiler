#pragma once

#include <print>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class NotEqual : public TAC
	{
	private:

		const size_t left {0};
		const size_t right {0};

	public:

		NotEqual(const size_t l, const size_t r, const std::string_view owning_proc)
			: TAC(owning_proc), left(l), right(r)
		{}
		virtual ~NotEqual() = default;

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
			// l == r --> (l - r) + (r - l) == 0 --> (r - l) + (l - r) == 0
			// because substraction is not associative

			REG temp = regalloc.allocTemporary(p_index);
			temp = regalloc.swap(temp);

			// RA = right
			regalloc.copy(p_index, p_index, right);
			temp = regalloc.swap(temp);
			REG left_reg = regalloc.getValue(left);
			REG right_reg = regalloc.getValue(right);
			
			// RA = (r - l)
			asm_table.add<ins::SUB>(left_reg);

			// RA = l
			left_reg = regalloc.swap(left_reg);
			temp = regalloc.get(p_index);
			// l => RA = (l - r)
			asm_table.add<ins::SUB>(right_reg);
			
			// RA = (r - l)
			temp = regalloc.swap(temp);
			
			// RA = (r - l) + (l - r)
			left_reg = regalloc.getValue(left);
			asm_table.add<ins::ADD>(left_reg);

			// RA = result
			regalloc.overrideRA(p_index);
		}

		virtual std::string __debug_string() const
		{
			return std::format("({}) != ({})", left, right);
		}
	};
	
} // namespace fl::tac
