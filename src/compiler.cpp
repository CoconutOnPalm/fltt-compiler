#include "compiler.hpp"

#include "utils/panic.hpp"
#include "utils/const.hpp"


namespace fl
{
    void Compiler::compile()
    {
		if (!m_procedure_map.contains(config::prog_entry_name))
		{
			panic("entry point not defined");
		}

		// make in-block variable declarations 
		// to add memory to 'for' loops' iterators
		for (auto& [name, proc] : m_procedure_map)
		{
			proc.makeInBlockDeclarations();
		}
		
		assignMemory();

		for (auto& [name, proc] : m_procedure_map)
		{
			proc.generateTAC(m_tac_table);
		}

		m_tac_table.generateASM();
    }


	void Compiler::defineProcedure(const std::string_view procedure_name, ast::ProcDecl* head, SymbolTable* symbol_table, ast::Block* body)
	{
		if (symbol_table == nullptr)
		{
			symbol_table = new SymbolTable;
		}
		if (body == nullptr)
		{
			body = new ast::Block;
		}

		// add a variable holding return adress
		symbol_table->add<Variable>("#return");
		
		if (head != nullptr)
		{
			// copy args into the symbol table
			const auto& args = head->getArgs();
			for (const Argument& arg : args)
			{
				symbol_table->addArgument(&arg); // this one adds arg to the arg table
				// symbol_table->add<Argument>(&arg);
			}
		}
		
		m_procedure_map.emplace(procedure_name, Procedure(procedure_name, symbol_table, body));
	}

	void Compiler::defineMain(SymbolTable* symbol_table, ast::Block* body)
	{
		defineProcedure(config::prog_entry_name, nullptr, symbol_table, body);
	}

	void Compiler::__debug_print() const
	{
		for (const auto& [name, procedure] : m_procedure_map)
		{
			procedure.__debug_print();
		}
	}

    void Compiler::assignMemory()
    {
		size_t next_free_address = 1; // 0 = null
		for (auto& [name, proc] : m_procedure_map)
		{
			next_free_address += proc.assignMemoryBlock(next_free_address);
		}

		// debug print
		for (auto& [name, proc] : m_procedure_map)
		{
			proc.__debug_print();
		}
    }


} // namespace fl
