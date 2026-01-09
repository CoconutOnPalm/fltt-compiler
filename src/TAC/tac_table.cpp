#include "tac_table.hpp"

#include <print>


namespace fl
{
    void TACTable::updateNextUse()
    {
		for (const auto& tac_ptr : m_tac_table)
		{
			tac_ptr->updateNextUse(m_tac_info);
		}
    }

    void TACTable::typeCheck(std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables)
    {
		for (const auto& tac_ptr : m_tac_table)
		{
			tac_ptr->typeCheck(m_tac_info, symbol_tables);
		}
    }

	
	void TACTable::generateASM() const
	{
		for (size_t i = 0; const auto& tac_ptr : m_tac_table)
		{
			std::print("{:2}| ", i); 
			tac_ptr->generateASM();
			i++;
		}
	}

    void TACTable::__debug_print() const
    {
		std::println("THREE ADRESS CODE");
		for (size_t i = 0; const auto& tac_ptr : m_tac_table)
		{
			std::println("  {:2}| {}", i++, tac_ptr->__debug_string());
		}

		std::println();
		std::println("NEXTUSE INFO");
		for (size_t i = 0; TACInfo tac_info : m_tac_info)
		{
			std::print("  {:2}| ", i++);
			auto usages = tac_info.usages();
			for (size_t usage : usages)
				std::print("{}, ", usage);
			// for (size_t j = 0; j < m_tac_info.size(); j++)
			// 	if (tac_info.hasNextUse(j))
			// 		std::print("{}, ", tac_info.nextUsage(j));
			std::println();
		}
    }
	
} // namespace fl
