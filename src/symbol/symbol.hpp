#pragma once

#include <string>
#include <cstdint>
#include <format>
#include <bitset>


namespace fl
{
	enum class SymbolType : uint8_t
	{
		NONE		= (0),
		VARIABLE 	= (1u << 0),
		ARGUMENT	= (1u << 1),
		ARRAY		= (1u << 2),
		IN			= (1u << 3),
		OUT			= (1u << 4),
	};
	
	class Symbol
	{
	public:

		const std::string name;
		const std::bitset<5> type_flags;
		
	protected:
		
		uint64_t assignment_counter {0};
		size_t memsize {0};
	
	public:
	
		Symbol(const std::string_view name, const size_t size, const uint8_t type);
		virtual ~Symbol() = default;
		
		virtual std::string __debug_string() const = 0;

	protected:

		std::string __flags_to_string() const;
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