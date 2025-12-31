#pragma once

#include "symbol.hpp"


namespace fl
{
	enum class ArgType
	{
		IN,
		OUT,
		ARRAY,
		NONE,
	};
	
	class Argument : Symbol
	{
	public:

		Argument(const std::string_view name, const ArgType arg_type);
		virtual ~Argument() = default;

		std::string __debug_string() const;
	
	private:

		static uint8_t mapArgTypes(const ArgType arg_type);
	};
	
} // namespace fl
