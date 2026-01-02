#include "compiler.hpp"

#include "utils/panic.hpp"
#include "utils/const.hpp"


namespace fl
{

	void Compiler::defineProcedure(const std::string_view procedure_name, SymbolTable* symbol_table, Block* body)
	{
		m_procedure_map.emplace(procedure_name, Procedure(procedure_name, symbol_table, body));
	}

	void Compiler::defineMain(SymbolTable* symbol_table, Block* body)
	{
		defineProcedure(config::prog_entry_name, symbol_table, body);
	}

	void Compiler::__debug_print() const
	{
		for (const auto& [name, procedure] : m_procedure_map)
		{
			procedure.__debug_print();
		}
	}


} // namespace fl
