#pragma once

#include <map>
#include <variant>

#include "symbol/symbol_table.hpp"
#include "procedure/procedure.hpp"
#include "tac/tac_table.hpp"


namespace fl
{
	
	class Compiler
	{
	private:

		TACTable m_tac_table_buffer;
		SymbolTable m_symbol_table_buffer;
		std::map<std::string, Procedure> m_procedure_map;

	public:

		Compiler() = default;
		~Compiler() = default;

		Compiler(const Compiler& other) = delete;
		Compiler& operator=(const Compiler& other) = delete;
		

		void pushProcedure(const std::string_view procedure_name);
		TACTableRef pushExpression(const Operator op, const tacval_t left, const tacval_t right);

		template <class SymT, typename... Args>
		inline void addSymbol(const std::string& name, Args&&... args)
		{
			m_symbol_table_buffer.add<SymT>(name, std::forward<Args>(args)...);
		}


		void __debug_print() const;


	private:

		void flushBuffers();

	};

} // namespace fl
