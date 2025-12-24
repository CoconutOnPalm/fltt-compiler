#include "array.hpp"

#include "../utils/panic.hpp"


namespace fl
{
	
	Array::Array(const std::string_view name, const uint64_t begin, const uint64_t end)
		: Symbol(name, (begin <= end) ? end - begin + 1 : 0)
		, begin(begin)
		, end(end)
	{
		if (begin > end)
		{
			panic("invalid array boundaries: [{}:{}]", begin, end);
		}
	}

    std::string Array::_debug_string() const
    {
        return std::format("{}[{}:{}] | size={}, assignments={}", name, begin, end, length, assignment_counter);
    }

} // namespace fl
