#include "variable.hpp"


namespace fl
{

    Variable::Variable(const std::string_view name)
		: Symbol(name, SymbolType::VARIABLE)
    {}

    std::string Variable::_debug_string() const
    {
        return std::format("{:20} \t| size={}, assignments={}", name, 1, assignment_counter);
    }

	
} // namespace fl
