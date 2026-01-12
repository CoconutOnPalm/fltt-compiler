#pragma once

#include <print>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class LessOrEqual : public TAC
	{
	private:

		const size_t left {0};
		const size_t right {0};

	public:

		LessOrEqual(const size_t l, const size_t r, const std::string_view owning_proc)
			: TAC(owning_proc), left(l), right(r)
		{}

		virtual ~LessOrEqual() = default;


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
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const
		{
			return std::format("({}) > ({})", left, right);
		}
	};
	
} // namespace fl::tac
