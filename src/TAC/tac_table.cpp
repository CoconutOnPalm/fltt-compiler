#include "tac_table.hpp"


namespace fl
{
	
	void TACTable::generateASM() const
	{
		for (const auto& tac_ptr : m_tac_table)
		{
			tac_ptr->generateASM();
		}
	}
	
} // namespace fl
