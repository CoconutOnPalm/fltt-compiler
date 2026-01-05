#pragma once

#include <print>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class NotEqual : public TAC
	{
	private:

		const size_t left {0};
		const size_t right {0};

	public:

		NotEqual(const size_t l, const size_t r)
			: left(l), right(r)
		{}
		virtual ~NotEqual() = default;

		virtual void generateASM() const 
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const
		{
			return std::format("({}) != ({})", left, right);
		}
	};
	
} // namespace fl::tac
