#pragma once

#include <string>
#include <cstdint>
#include <format>


namespace fl
{
	enum class SymbolType
	{
		VARIABLE,
		ARRAY
	};
	
	class Symbol
	{
	public:

		const std::string name;
		const SymbolType type;
		
	protected:
		
		uint64_t assignment_counter {0};
	
	public:
	
		Symbol(const std::string_view name, const SymbolType type);
		virtual ~Symbol() = default;
		
		virtual std::string _debug_string() const = 0;
	};

} // namespace fl


// template <typename SymChildT, typename char_T>
// struct std::formatter<SymChildT, std::enable_if_t<std::is_base_of<fl::Symbol, SymChildT>::value,  char_T>> : std::formatter<std::string>
// {
// 	auto format(const SymChildT& symbol, std::format_context& ctx) const
// 	{
// 		return std::formatter<std::string>::format(std::format("{}", symbol._debug_string()), ctx);
// 	}
// };