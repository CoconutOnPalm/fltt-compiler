#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Index : public TAC
	{

		size_t lval;
		size_t rval;

	public:

		Index(const size_t lvalue, const size_t rvalue)
			: lval(lvalue), rval(rvalue)
		{}

		virtual ~Index() = default;

		virtual void generateASM() const override
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({})[({})]", lval, rval);
		}
	};
	
} // namespace fl
