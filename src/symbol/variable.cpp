#include "variable.hpp"


namespace fl
{

    Variable::Variable(const std::string_view name)
		: Symbol(name, 1, static_cast<uint8_t>(SymbolType::VARIABLE))
    {}

    std::string Variable::__debug_string() const
    {
        return std::format("{:10} \t| size={}, assignments={} | {}", name, 1, assignment_counter, __flags_to_string());
    }

	
} // namespace fl
