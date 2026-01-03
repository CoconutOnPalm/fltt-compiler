#include "symbol_table.hpp"


namespace fl
{
	const Symbol& SymbolTable::get(const std::string& name) const
	{
		if (!m_symbol_table.contains(name))
		{
			panic("undefined variable '{}'", name);
		}

		return *(m_symbol_table.at(name).get());
	}


	void SymbolTable::assignMemory(const size_t shift)
	{
		size_t relative_pos = 0;
		for (auto& [name, symb_ptr] : m_symbol_table)
		{
			if (symb_ptr->testFlag(SymbolType::ARGUMENT))
				continue;

			symb_ptr->setMemoryPosition(shift + relative_pos);
			relative_pos += symb_ptr->memsize;
		}
	}

	void SymbolTable::__debug_print() const
	{
		for (const auto& [name, symbol] : m_symbol_table)
		{
			std::println("{}", symbol->__debug_string());
		}
	}

} // namespace fl

