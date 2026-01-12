#pragma once

#include <string>
#include <cstdint>
#include <format>
#include <bitset>
#include <memory>


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
		const size_t memsize;
		
	protected:
		
		uint64_t m_assignment_counter {0};
		uint64_t m_usage_counter {0};

		size_t m_mempos {0};
	
	public:
	
		Symbol(const std::string_view name, const size_t size, const uint8_t type);
		virtual ~Symbol() = default;

		virtual std::unique_ptr<Symbol> clone() const = 0;

		void noticeAssign();
		uint64_t assignments() const;

		void noticeUse();
		uint64_t usages() const;

		virtual void setMemoryPosition(const size_t position);
		virtual size_t address() const { return m_mempos; }
		
		bool testFlag(const SymbolType type) const;
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