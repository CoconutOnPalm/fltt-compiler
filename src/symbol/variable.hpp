#pragma once

#include "symbol.hpp"


namespace fl
{
	
	class Variable : public Symbol
	{
	private:

	public:

		Variable(const std::string_view name, const SymbolType custom_st_flags = SymbolType::NONE);
		~Variable() = default;

		std::unique_ptr<Symbol> clone() const override;

		std::string __debug_string() const;
	};

} // namespace fl


template <typename char_T>
struct std::formatter<fl::Variable, char_T> : std::formatter<std::string>
{
	auto format(const fl::Variable& symbol, std::format_context& ctx) const
	{
		return std::formatter<std::string>::format(std::format("{}", symbol.__debug_string()), ctx);
	}
};