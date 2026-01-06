#pragma once

#include <print>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Inc : public TAC
	{
	private:

		const size_t lval;
	
	public:

		Inc(const size_t lvalue)
			: lval(lvalue)
		{}

		virtual ~Inc() = default;

		virtual void generateASM() const override
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({})++", lval);
		}
		
	};
	
} // namespace fl
