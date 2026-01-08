#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Assign : public TAC
	{

		size_t lval;
		size_t rval;

	public:

		Assign(const size_t lvalue, const size_t rvalue)
			: lval(lvalue), rval(rvalue)
		{}

		virtual ~Assign() = default;

		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::ASSIGNMENT);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[lval].useIn(p_index);
			info_table[rval].useIn(p_index);
		}

		void typeCheck(const std::vector<TACInfo>& info_table) const override
		{
			if (info_table[lval].code_type != TACType::VARIABLE)
				panic("illegal operation - rvalue assignment");
		}

		virtual void generateASM() const override
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({}) = ({})", lval, rval);
		}
	};
	
} // namespace fl
