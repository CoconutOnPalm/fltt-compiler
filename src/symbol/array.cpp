#include "array.hpp"

#include "../utils/panic.hpp"


namespace fl
{
	
	Array::Array(const std::string_view name, const uint64_t begin, const uint64_t end)
		: Symbol(name, SymbolType::ARRAY)
		, begin(begin), end(end)
		, size(end - begin + 1)
	{
		if (begin > end)
		{
			panic("invalid array boundaries: [{}:{}]", begin, end);
		}
	}

    std::string Array::_debug_string() const
    {
        return std::format("{:20} | size={}, assignments={}", std::format("{}[{}:{}]", name, begin, end), size, assignment_counter);
    }

} // namespace fl
