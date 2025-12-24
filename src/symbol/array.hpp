#pragma once

#include "symbol.hpp"


namespace fl
{
	
	class Array : public Symbol
	{
	private:

		const uint64_t begin, end;

	public:

		Array(const std::string_view name, const uint64_t begin, const uint64_t end);
		virtual ~Array() = default;

		std::string _debug_string() const;
	};

} // namespace fl


template <typename char_T>
struct std::formatter<fl::Array, char_T> : std::formatter<std::string>
{
	auto format(const fl::Array& symbol, std::format_context& ctx) const
	{
		return std::formatter<std::string>::format(std::format("{}", symbol._debug_string()), ctx);
	}
};