#include "symbol.hpp"

#include <vector>


namespace fl
{
	
	Symbol::Symbol(const std::string_view name, const size_t size, const uint8_t type)
		: name(name), memsize(size), type_flags(type)
	{}

    void Symbol::setRelativeMemoryPosition(const size_t position)
    {
		mempos = position;
    }

    std::string Symbol::__flags_to_string() const
    {
		const static std::vector<std::string> mapper {
					// SymbolType::NONE		
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