#include "array.hpp"

#include "../utils/panic.hpp"


namespace fl
{

	Array::Array(const std::string_view name, const uint64_t begin, const uint64_t end)
		: Symbol(name, header_size + (end - begin + 1), static_cast<uint8_t>(SymbolType::ARRAY))
		, begin(begin), end(end)
	{
		if (begin > end)
		{
			panic("invalid array boundaries: [{}:{}]", begin, end);
		}
	}

	std::unique_ptr<Symbol> Array::clone() const
	{
		return std::make_unique<Array>(*this);
	}

	std::string Array::__debug_string() const
	{
		return std::format("{} | memsize={}, mempos={} | {}", std::format("{}[{}:{}]", name, begin, end), memsize, m_mempos, __flags_to_string());
	}

} // namespace fl
