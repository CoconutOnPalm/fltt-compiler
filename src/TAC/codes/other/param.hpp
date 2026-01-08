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

		const size_t index;
		uint64_t param {0};
		const std::string dest;

	public:

		Param(const uint64_t param, const std::string_view destination, const size_t argindex)
			: param(param), dest(destination), index(argindex)
		{}

		virtual ~Param() = default;

		
		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::PARAM);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[param].useIn(p_index);
		}

		void typeCheck(const std::vector<TACInfo>& info_table, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			if (!symbol_tables.contains(dest))
				panic("undefined procedure '{}'", dest);

			TACInfo this_param = info_table[param];

			SymbolTable& st = *(symbol_tables[dest]);
			std::string arg_id = st.argAt(index);
			Symbol& arg = st[arg_id];
			
			if (!arg.testFlag(SymbolType::ARGUMENT))
				panic("internal compiler error: Param::typeCheck - symbol is not an argument");

			if (arg.testFlag(SymbolType::IN))
			{
				if (this_param.code_type != TACType::CONSTANT)
					panic("param is not an IN argument");
			}
		}

		
		virtual void generateASM() const 
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const
		{
			return std::format("param ({}) => {}", param, dest);
		}
	};
	
} // namespace fl
