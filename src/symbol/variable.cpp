#include "variable.hpp"


namespace fl
{

    Variable::Variable(const std::string_view name)
		: Symbol(name, 1, static_cast<uint8_t>(SymbolType::VARIABLE))
    {}

    std::unique_ptr<Symbol> Variable::clone() const
    {
        return std::make_unique<Variable>(*this);
    }

std::string Variable::__debug_string() const
    {
        return std::format("{} \t| memsize={}, mempos={} | {}", name, memsize, m_mempos, __flags_to_string());
    }

	
} // namespace fl
