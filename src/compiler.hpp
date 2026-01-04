#pragma once

#include <map>
#include <variant>

#include "symbol/symbol_table.hpp"
#include "procedure/procedure.hpp"
#include "AST/blocks/proc_decl.hpp"
#include "TAC/tac_table.hpp"


namespace fl
{
	
	class Compiler
	{
	private:

		std::map<std::string, Procedure> m_procedure_map;
		TACTable m_tac_table;

	public:

		Compiler() = default;
		~Compiler() = default;

		Compiler(const Compiler& other) = delete;
		Compiler& operator=(const Compiler& other) = delete;

		void compile();
		
		void defineProcedure(const std::string_view procedure_name, ast::ProcDecl* head, SymbolTable* symbol_table, ast::Block* body);
		void defineMain(SymbolTable* symbol_table, ast::Block* body);

		void __debug_print() const;


	private:

		void assignMemory();

	};

} // namespace fl
