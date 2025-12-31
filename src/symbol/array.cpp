#include "array.hpp"

#include "../utils/panic.hpp"


namespace fl
{
	
	Array::Array(const std::string_view name, const uint64_t begin, const uint64_t end)
		: Symbol(name, (end - begin + 1), static_cast<uint8_t>(SymbolType::ARRAY))
		, begin(begin), end(end)
	{
		if (begin > end)
		{
			panic("invalid array boundaries: [{}:{}]", begin, end);
		}
	}

    std::string Array::__debug_string() const
    {
        return std::format("{:10} | size={}, assignments={} | {}", std::format("{}[{}:{}]", name, begin, end), memsize, assignment_counter, __flags_to_string());
    }

} // namespace fl
