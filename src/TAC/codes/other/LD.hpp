#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class LD : public TAC
	{

		std::string identifier;

	public:

		LD(const std::string_view id, const std::string_view owning_proc)
			: TAC(owning_proc), identifier(id)
		{}

		virtual ~LD() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::VARIABLE, p_owning_procedure, identifier);
		}
		
		virtual void generateASM() const override
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("LD '{}'", identifier);
		}
	};
	
} // namespace fl
