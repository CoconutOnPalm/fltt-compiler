#include "symbol_table.hpp"


namespace fl
{
	Symbol& SymbolTable::get(const std::string& name)
	{
		if (!m_symbol_table.contains(name))
		{
			panic("undefined variable '{}'", name);
		}

		return *(m_symbol_table.at(name).get());
	}

    std::string SymbolTable::argAt(const size_t index)
    {
		if (index >= m_arg_ids.size())
			panic("procedure called with too many arguments");

		return m_arg_ids[index];
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

