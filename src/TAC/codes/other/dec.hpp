#pragma once

#include <print>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Dec : public TAC
	{
	private:

		const size_t lval;
	
	public:

		Dec(const size_t lvalue, const std::string_view owning_proc)
			: TAC(owning_proc), lval(lvalue)
		{}

		virtual ~Dec() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[lval].useIn(p_index);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({})--", lval);
		}
		
	};
	
} // namespace fl
