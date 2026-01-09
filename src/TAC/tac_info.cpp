#include "tac_info.hpp"

#include "../utils/logging.hpp"
#include "../utils/panic.hpp"


namespace fl
{
	
	void fl::TACInfo::useIn(const size_t tac_index)
	{
		m_usages.push_back(tac_index);
	}

    size_t TACInfo::nextUsage(const size_t current_tac_index)
    {
		for (const size_t usage : m_usages)
			if (usage > current_tac_index)
				return usage;	
	
		warning("nextUsage(): no further usage registered");
		return -1;
    }

    bool TACInfo::hasNextUse(const size_t current_tac_index) const
    {
		for (const size_t usage : m_usages)
			if (usage > current_tac_index)
				return true;	

		return false;
    }

    std::vector<size_t> TACInfo::usages() const
    {
        return m_usages;
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
		case TACType::ARRELEM:
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

