#include "argument.hpp"


#include "../utils/panic.hpp"


namespace fl
{

	Argument::Argument(const std::string_view name, const ArgType arg_type)
		: Symbol(name, 0, mapArgTypes(arg_type))
	{}

    std::string Argument::__debug_string() const
    {
        return std::format("{:10} \t| size={}, assignments={} | ", name, 1, assignment_counter, __flags_to_string());
    }

    
	uint8_t Argument::mapArgTypes(const ArgType arg_type)
    {
		uint8_t flags = static_cast<uint8_t>(SymbolType::ARGUMENT);
		switch (arg_type)
		{
		case ArgType::IN:
			flags |= static_cast<uint8_t>(SymbolType::IN);
		break;
		case ArgType::OUT:
			flags |= static_cast<uint8_t>(SymbolType::OUT);
		break;
		case ArgType::ARRAY:
			flags |= static_cast<uint8_t>(SymbolType::ARRAY);
		break;
		case ArgType::NONE:
			// no type
		break;
		default:
			panic("internal compiler error: unknown ArgType inside Argument::mapArgTypes");
		}

		return flags;
    }
	
} // namespace fl