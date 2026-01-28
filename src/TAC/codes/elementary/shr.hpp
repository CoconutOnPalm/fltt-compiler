#pragma once

#include <print>

#include "../../tac.hpp"

#include "../../../ASM/instructions/shr.hpp"


namespace fl::tac
{

	// WARNING: use only on temporaries
	class Shr : public TAC
	{
	private:

		const size_t code;
	
	public:

		Shr(const size_t code, const std::string_view owning_proc)
			: TAC(owning_proc), code(code)
		{}

		virtual ~Shr() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{

		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, const std::vector<TACInfo>& info_table) const override
		{	
			REG reg = regalloc.getValue(code);
			reg = regalloc.swap(reg); // we have to swap due to the behaviour of other TACs
			asm_table.add<ins::SHR>(reg);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({}) >> 1", code);
		}
		
	};
	
} // namespace fl
