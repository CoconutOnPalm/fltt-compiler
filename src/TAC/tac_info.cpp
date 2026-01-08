#include "tac_info.hpp"

#include "../utils/logging.hpp"
#include "../utils/panic.hpp"


namespace fl
{
	
	void fl::TACInfo::useIn(const size_t tac_index)
	{
		m_next_use.push(tac_index);
	}

    size_t TACInfo::nextUsage()
    {
		if (m_next_use.empty())
		{
			warning("nextUsage(): no further usage registered");
			return -1;
		}

        const size_t next = m_next_use.front();
		m_next_use.pop();
		return next;
    }

    bool TACInfo::hasNextUse() const
    {
        return !m_next_use.empty();
    }

    bool TACInfo::mapBlockTerminators(const TACType type)
    {
		switch (type)
		{
		case TACType::ASSIGNMENT:
			return false;
		case TACType::CONDITION:
			return false;
		case TACType::CONSTANT:
			return false;
		case TACType::EXPRESSION:
			return false;
		case TACType::IO:
			return false;
		case TACType::PARAM:
			return false;
		case TACType::VARIABLE:
			return false;
		case TACType::JUMP:	// terminator
			return true;
		case TACType::LABEL: // terminator
			return true;
		default:
			panic("internal compiler error: unknown TACType in block terminator mapper");
			return false; // for g++ to shut up
		}
    }

} // namespace fl

