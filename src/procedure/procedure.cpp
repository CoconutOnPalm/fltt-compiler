#include "procedure.hpp"

namespace fl
{
	


    Procedure::Procedure(const std::string_view name, SymbolTable* symbol_table_ptr, Block* body_ptr)
		: name(name), m_symbol_table(symbol_table_ptr), m_body(body_ptr), m_call_count(0)
    {
		
	}

    uint64_t Procedure::callCount() const
    {
		return m_call_count;
    }

    void Procedure::__debug_print() const
    {
		std::println("func {}(???)", name);
		m_symbol_table->__debug_print();
		m_body->generateTAC(); // debug print
    }

} // namespace fl
