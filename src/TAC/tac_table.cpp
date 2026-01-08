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
			while (tac_info.hasNextUse())
				std::print("{}, ", tac_info.nextUsage());
			std::println();
		}
    }
	
} // namespace fl
