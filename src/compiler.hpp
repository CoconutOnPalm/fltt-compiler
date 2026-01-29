#pragma once

#include <map>
#include <memory>
#include <filesystem>

#include "symbol/symbol_table.hpp"
#include "procedure/procedure.hpp"
#include "AST/blocks/proc_decl.hpp"
#include "TAC/tac_table.hpp"
#include "ASM/asm_table.hpp"

namespace fl
{
	
	class Compiler
	{
	private:

		std::map<std::string, Procedure> m_procedure_map;
		std::map<std::string, std::shared_ptr<SymbolTable>> m_symbol_tables;
		TACTable m_tac_table;
		std::shared_ptr<ASMTable> m_asm_table;

	public:

		Compiler();
		~Compiler() = default;

		Compiler(const Compiler& other) = delete;
		Compiler& operator=(const Compiler& other) = delete;

		void compile(const std::filesystem::path& outpath);
		
		void defineProcedure(const std::string_view procedure_name, ast::ProcDecl* head, SymbolTable* symbol_table, ast::Block* body);
		void defineMain(SymbolTable* symbol_table, ast::Block* body);

		void __debug_print() const;


	private:

		size_t assignMemory();
		void generateTAC();
		void generateProgram(const std::filesystem::path& outpath);

	};

} // namespace fl
