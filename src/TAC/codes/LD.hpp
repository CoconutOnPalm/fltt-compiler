#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../tac.hpp"


namespace fl::tac
{
	
	class LD : public TAC
	{

		std::string identifier;

	public:

		LD(const std::string_view id)
			: identifier(id)
		{}

		virtual ~LD() = default;

		virtual void generateASM() const 
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const
		{
			return std::format("'{}'", identifier);
		}
	};
	
} // namespace fl
