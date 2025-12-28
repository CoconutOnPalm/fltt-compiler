#include "symbol.hpp"


namespace fl
{
	
	Symbol::Symbol(const std::string_view name, const SymbolType type)
		: name(name), type(type)
	{}
	
} // namespace fl