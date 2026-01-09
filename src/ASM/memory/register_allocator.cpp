#include "register_allocator.hpp"

#include "../../utils/panic.hpp"


namespace fl
{
    RegAlloc::RegAlloc(std::shared_ptr<std::vector<TACInfo>> tac_info, const size_t stack_ptr)
		: m_tac_info(tac_info), m_stack_ptr(stack_ptr)
    {}

	void RegAlloc::allocate(const size_t tac)
	{
		allocate(tac, "");
	}

    void RegAlloc::allocate(const size_t tac, const std::string_view variable_name)
    {
		// search for this tac 
		for (size_t i = 0; i < m_registers.size(); i++)
		{
			if (m_registers[i].tac_index == tac)
			{
				return;
				// return static_cast<REG>(i);
			}
		}

		// search for the first empty register
		for (size_t i = 0; i < m_registers.size(); i++)
		{
			Register& reg = m_registers[i];
			if (reg.tac_index == empty_tac)
			{
				reg.tac_index = tac;
				reg.assoc_variable = variable_name;
				return;
				// return static_cast<REG>(i);
			}
		}

		std::vector<TACInfo>& tac_info = *m_tac_info; // alias
		// search for the first dead tac
		for (size_t i = 0; i < m_registers.size(); i++)
		{
			Register& reg = m_registers[i];
			if (reg.tac_index >= tac_info.size())
				panic("internal compier error: requested a register for TAC beyond scope");

			if (!tac_info[reg.tac_index].hasNextUse(tac))
			{
				// reassign register and forget the old one
				reg.tac_index = tac;
				reg.assoc_variable = variable_name;
				return;
				// return static_cast<REG>(i);
			}
		}

		panic("out of registers");
    }

    void RegAlloc::swap(const REG reg)
    {
		if (reg == REG::RA)
			return;

		std::swap(m_registers[0], m_registers[static_cast<size_t>(reg)]);
    }

    REG RegAlloc::get(const size_t tac)
    {
		for (size_t i = 0; i < m_registers.size(); i++)
		{
			if (m_registers[i].tac_index == tac)
				return static_cast<REG>(i);
		}

		panic("register lost for TAC ({})", tac);
		return REG::RA;
    }
	
} // namespace fl

