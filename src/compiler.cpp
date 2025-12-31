#include "compiler.hpp"

#include "utils/panic.hpp"


namespace fl
{
	
    void Compiler::pushProcedure(const std::string_view procedure_name)
    {
		std::println("pushProcedure()");
		if (m_procedure_map.contains(std::string{procedure_name}))
		{
			panic("redeclaration of the procedure '{}'", procedure_name);
		}

		m_procedure_map.try_emplace(std::string{procedure_name}, procedure_name, std::move(m_symbol_table_buffer));
		flushBuffers();
    }

	
	void Compiler::flushBuffers()
    {
		m_symbol_table_buffer.clear();
    }
	


	void Compiler::__debug_print() const
	{
		for (const auto& [name, procedure] : m_procedure_map)
		{
			std::vector<std::string> declared_variables = procedure.symbol_table.getDeclaredVariables();
			
			std::println("{}:", name);

			std::println("  declared:");
			for (const std::string& symbol : declared_variables)
			{
				std::println("    {}", procedure.symbol_table[symbol].__debug_string());
				
			}
		}
	}


} // namespace fl
