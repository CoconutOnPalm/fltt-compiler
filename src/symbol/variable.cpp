#include "variable.hpp"


namespace fl
{

    Variable::Variable(const std::string_view name)
		: Symbol(name, 1)
    {}

    std::string Variable::_debug_string() const
    {
        return std::format("{} \t| size={}, assignments={}", name, length, assignment_counter);
    }

	
} // namespace fl
