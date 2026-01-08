#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class LDC : public TAC
	{

		std::string identifier;

	public:

		LDC(const std::string_view id)
			: identifier(id)
		{}

		virtual ~LDC() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::CONSTANT);
		}
		
		virtual void generateASM() const override
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("LDC '{}'", identifier);
		}
	};
	
} // namespace fl
