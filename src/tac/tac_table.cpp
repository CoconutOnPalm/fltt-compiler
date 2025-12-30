#include "tac_table.hpp"


namespace fl
{
	
    TACTable::TACTable()
		: counter(0)
    {
		
    }

    TACTableRef TACTable::pushExpression(const Operator op, const tacval_t left, const tacval_t right)
    {
		tac.emplace_back(Triple {
			.op = op, 
			.left = left, 
			.right = right, 
		});

		TACTableRef entry {
			.index = counter, 
		};
		counter++;

		ids.push_back(entry);

		return entry;
    }

    void TACTable::addSymbolTable(const SymbolTable& symbol_table)
    {
    }

    void TACTable::clear()
    {
		tac.clear();
		ids.clear();
		counter = 0;
    }

    void TACTable::_debug_print() const
    {
		for (size_t i = 0; i < tac.size(); i++)
		{
			std::println("{:2}| {}", ids[i].index, tac[i]);
		}
    }

} // namespace fl
