#include "compiler.hpp"

#include "utils/panic.hpp"


namespace fl
{
	
    void Compiler::pushProcedure(const std::string_view procedure_name)
    {
		if (m_procedure_map.contains(std::string{procedure_name}))
		{
			panic("redeclaration of the procedure '{}'", procedure_name);
		}

		// m_procedure_map[std::string{procedure_name}] = Procedure(procedure_name, std::move(m_symbol_table_buffer));
		// m_procedure_map.emplace(std::make_pair<std::string, Procedure>(std::string{procedure_name}, Procedure(procedure_name, std::move(m_symbol_table_buffer))));

		m_procedure_map.try_emplace(std::string{procedure_name}, procedure_name, std::move(m_symbol_table_buffer));


		flushSymbolTable();
    }

	
	void Compiler::flushSymbolTable()
    {
		m_symbol_table_buffer.clear();
    }
	


	void Compiler::__debug_print() const
	{
		for (const auto& [name, procedure] : m_procedure_map)
		{
			std::println("{}:", name);
			std::vector<std::string> declared_variables = procedure.symbol_table.getDeclaredVariables();
			for (const std::string& symbol : declared_variables)
			{
				std::println("    {}", procedure.symbol_table[symbol]._debug_string());
			}
		}
	}


} // namespace fl
