#include "tac_table.hpp"

#include <print>


namespace fl
{
	
	void TACTable::generateASM() const
	{
		for (size_t i = 0; const auto& tac_ptr : m_tac_table)
		{
			std::print("{:2}| ", i); 
			tac_ptr->generateASM();
			i++;
		}
	}
	
} // namespace fl
