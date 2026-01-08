#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Write : public TAC
	{
	private:

	public:

		Write() = default;
		virtual ~Write() = default;

		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::IO);
		}
		
		virtual void generateASM() const 
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const
		{
			return "WRITE";
		}
	};
	
} // namespace fl
