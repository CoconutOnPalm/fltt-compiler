#pragma once

#include <map>
#include <variant>

#include "symbol/symbol_table.hpp"
#include "procedure/procedure.hpp"


namespace fl
{
	
	class Compiler
	{
	private:

		SymbolTable m_symbol_table_buffer;
		std::map<std::string, Procedure> m_procedure_map;

	public:

		Compiler() = default;
		~Compiler() = default;

		Compiler(const Compiler& other) = delete;
		Compiler& operator=(const Compiler& other) = delete;
		

		void pushProcedure(const std::string_view procedure_name);

		template <typename Stmt_T, typename... Args>
		inline size_t pushStatement(Args&&... args)
		{
			return 0;
		}

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
