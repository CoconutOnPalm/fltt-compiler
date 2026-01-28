#pragma once

#include <print>

#include "../../tac.hpp"

#include "../../../ASM/instructions/store.hpp"


namespace fl::tac
{
	
	// WARNING: use only on temporaries
	class IterInc : public TAC
	{
	private:

		const size_t code;
	
	public:

		IterInc(const size_t code, const std::string_view owning_proc)
			: TAC(owning_proc), code(code)
		{}

		virtual ~IterInc() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{

		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, const std::vector<TACInfo>& info_table) const override
		{	
			panic("not implemented");
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({})++", code);
		}
		
	};
	
} // namespace fl
