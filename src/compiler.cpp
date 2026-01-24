#include "compiler.hpp"

#include <fstream>
#include <limits>

#include "utils/panic.hpp"
#include "utils/const.hpp"

#include "ASM/memory/register_allocator.hpp"

namespace fl
{
    Compiler::Compiler()
		: m_asm_table(std::make_shared<ASMTable>())
    {}

    void Compiler::compile(const std::filesystem::path& outpath)
    {	
		if (!m_procedure_map.contains(config::prog_entry_name))
		{
			panic("entry point not defined / syntax error");
		}

		// make in-block variable declarations 
		// to add memory to 'for' loops' iterators
		for (auto& [name, proc] : m_procedure_map)
		{
			proc.makeInBlockDeclarations();
		}
		
		size_t stack_ptr = assignMemory();
		
		m_procedure_map.at(config::prog_entry_name).generateTAC(m_tac_table);

		for (auto& [name, proc] : m_procedure_map)
		{
			if (name != config::prog_entry_name)
				proc.generateTAC(m_tac_table);
		}
		
		m_tac_table.updateNextUse();
		
		m_tac_table.__debug_print();
		m_tac_table.typeCheck(m_symbol_tables);

		m_tac_table.generateASM(m_asm_table, stack_ptr, m_symbol_tables);


		std::ofstream outfstream(outpath);
		if (!outfstream.good())
			panic("could not produce output file");
		
		std::println("ASM: ");
		m_asm_table->generate(outfstream);
    }


	void Compiler::defineProcedure(const std::string_view procedure_name, ast::ProcDecl* head, SymbolTable* symbol_table, ast::Block* body)
	{
		static uint64_t proc_uuid = std::numeric_limits<uint64_t>::max();

		if (symbol_table == nullptr)
		{
			symbol_table = new SymbolTable;
		}
		if (body == nullptr)
		{
			body = new ast::Block;
		}

		// add a variable holding return adress
		symbol_table->setUUID(proc_uuid--);
		symbol_table->add<Variable>(config::return_variable_name);
		
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
		
		
		body->setOwner(std::string{procedure_name});
		std::shared_ptr<SymbolTable> st_smartptr(symbol_table);
		m_symbol_tables.emplace(procedure_name, st_smartptr);
		m_procedure_map.emplace(procedure_name, Procedure(procedure_name, st_smartptr, body));
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

    size_t Compiler::assignMemory()
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

		return next_free_address;
    }


} // namespace fl
