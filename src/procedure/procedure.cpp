#include "procedure.hpp"

namespace fl
{
	


    Procedure::Procedure(const std::string_view name, SymbolTable&& symb_table)
		: name(name), symbol_table(std::move(symb_table)), m_call_count(0)
    {
		
	}

    uint64_t Procedure::callCount() const
    {
		return m_call_count;
    }

} // namespace fl
