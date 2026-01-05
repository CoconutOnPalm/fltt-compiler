#pragma once

#include <print>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class GreaterOrEqual : public TAC
	{
	private:

		const size_t left {0};
		const size_t right {0};

	public:

		GreaterOrEqual(const size_t l, const size_t r)
			: left(l), right(r)
		{}

		virtual ~GreaterOrEqual() = default;

		virtual void generateASM() const 
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const
		{
			return std::format("({}) > ({})", left, right);
		}
	};
	
} // namespace fl
