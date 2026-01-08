#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Call : public TAC
	{
	private:

		const std::string callee;

	public:

		Call(const std::string_view callee)
			: callee(callee)
		{}

		virtual ~Call() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::JUMP);
		}
		
		virtual void generateASM() const 
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const
		{
			return std::format("call @{}", callee);
		}
	};
	
} // namespace fl
