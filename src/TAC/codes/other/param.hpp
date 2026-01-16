#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Param : public TAC
	{

		const std::string param;
		const size_t index;
		const std::string dest;

	public:

		Param(const std::string_view param_id, const std::string_view destination, const size_t argindex, const std::string_view owning_proc)
			: TAC(owning_proc), param(param_id), dest(destination), index(argindex)
		{}

		virtual ~Param() = default;

		
		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::PARAM, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{}

		void typeCheck(const std::vector<TACInfo>& info_table, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			if (!symbol_tables.contains(dest))
				panic("undefined procedure '{}'", dest);
			
			std::string arg_id = symbol_tables[dest]->argAt(index);
			Symbol& arg = symbol_tables[dest]->get(arg_id);

			Symbol& this_param = symbol_tables[p_owning_procedure]->get(param);
			
			if (!arg.testFlag(SymbolType::ARGUMENT))
				panic("internal compiler error: Param::typeCheck - symbol '{}' is not an argument", arg.name);

			if (arg.testFlag(SymbolType::IN))
			{
				if (p_owning_procedure.empty())
					panic("internal compiler error: no owning procedure assigned");
				if (this_param.testFlag(SymbolType::OUT))
					panic("cannot pass an OUT variable ({}) to an IN param ({})", param, arg.name);
				// if (symbol_tables[p_owning_procedure]->get(this_param.associated_variable).testFlag(SymbolType::ARGUMENT) && this_param.code_type != TACType::CONSTANT)
				// 	panic("param '{}' is not an IN argument", this_param.associated_variable);
			}
		}

		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override 
		{
			std::shared_ptr<SymbolTable> callee = symbol_tables[dest];
			std::string arg_identifier = callee->argAt(index);
			size_t passed_address = callee->get(arg_identifier).address();

			size_t address = symbol_tables[p_owning_procedure]->get(param).address();

			const REG reg = regalloc.loadImmediate(p_index, address);
			regalloc.storeVariable(reg, passed_address);
		}

		virtual std::string __debug_string() const
		{
			return std::format("param ({}) => {}", param, dest);
		}
	};
	
} // namespace fl
