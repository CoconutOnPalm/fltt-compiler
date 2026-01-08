#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Read : public TAC
	{
	private:

	public:

		Read() = default;
		virtual ~Read() = default;

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
			return "READ";
		}
	};
	
} // namespace fl
