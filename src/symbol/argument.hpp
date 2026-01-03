#pragma once

#include "symbol.hpp"


namespace fl
{
	enum ArgType : int
	{
		IN,
		OUT,
		ARRAY,
		NONE,
	};
	
	class Argument : public Symbol
	{
	public:

		Argument(const std::string_view name, const ArgType arg_type);
		Argument(const Argument& other) = default;
		virtual ~Argument() = default;

		std::unique_ptr<Symbol> clone() const override;

		std::string __debug_string() const;
	
	private:

		static uint8_t mapArgTypes(const ArgType arg_type);
	};
	
} // namespace fl


