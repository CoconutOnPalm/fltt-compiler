#include "symbol.hpp"

#include <vector>


namespace fl
{

	Symbol::Symbol(const std::string_view name, const size_t size, const uint8_t type)
		: name(name), memsize(size), type_flags(type)
	{
	}

    void Symbol::noticeAssign()
    {
		m_assignment_counter++;
    }

    uint64_t Symbol::assignments() const
    {
        return m_assignment_counter;
    }

    void Symbol::noticeUse()
    {
		m_usage_counter++;
    }

    uint64_t Symbol::usages() const
    {
        return m_usage_counter;
    }

	void Symbol::setMemoryPosition(const size_t position)
	{
		m_mempos = position;
	}

	bool Symbol::testFlag(const SymbolType type) const
	{
		if (type == SymbolType::NONE)
			return type_flags.any();

		return (type_flags.to_ulong() & static_cast<ulong>(type)) != 0u;
	}

	std::string Symbol::__flags_to_string() const
	{
		const static std::vector<std::string> mapper{
			"VAR",	// SymbolType::VARIABLE
			"ARG",	// SymbolType::ARGUMENT	
			"T",	// SymbolType::ARRAY		
			"I",	// SymbolType::IN			
			"O"		// SymbolType::OUT			
		};

		std::string str;
		for (uint8_t i = 0; i < type_flags.size(); i++)
		{
			if (type_flags.test(i))
				str += std::format("{} |", mapper[i]);
		}
		return str;
	}

} // namespace fl