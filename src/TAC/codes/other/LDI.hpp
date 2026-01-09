#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class LDI : public TAC
	{

		uint64_t value {0};

	public:

		LDI(const uint64_t immediate, const std::string_view owning_proc)
			: TAC(owning_proc), value(immediate)
		{}

		virtual ~LDI() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::CONSTANT, p_owning_procedure);
		}
		
		virtual void generateASM() const 
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const
		{
			return std::format("LDI {}", value);
		}
	};
	
} // namespace fl
