#include "symbol.hpp"


namespace fl
{
	
	Symbol::Symbol(const std::string_view name, const uint64_t length)
		: name(name), length(length)
	{}
	
} // namespace fl