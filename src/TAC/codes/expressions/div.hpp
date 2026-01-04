#pragma once

#include <print>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Div : public TAC
	{
	private:

		const size_t left_index;
		const size_t right_index;
	
	public:

		Div(const size_t left, const size_t right)
			: left_index(left), right_index(right)
		{}

		virtual ~Div() = default;

		virtual void generateASM() const override
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({}) / ({})", left_index, right_index);
		}
		
	};
	
} // namespace fl
