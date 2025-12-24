#pragma once

#include "symbol.hpp"


namespace fl
{
	
	class Variable : public Symbol
	{
	public:

		Variable(const std::string_view name);
		~Variable() = default;

		std::string _debug_string() const;
	};

} // namespace fl


template <typename char_T>
struct std::formatter<fl::Variable, char_T> : std::formatter<std::string>
{
	auto format(const fl::Variable& symbol, std::format_context& ctx) const
	{
		return std::formatter<std::string>::format(std::format("{}", symbol._debug_string()), ctx);
	}
};