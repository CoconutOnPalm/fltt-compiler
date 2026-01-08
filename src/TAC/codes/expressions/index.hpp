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
		size_t index;

	public:

		Index(const size_t lvalue, const size_t ind)
			: lval(lvalue), index(ind)
		{}

		virtual ~Index() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[lval].useIn(p_index);
			info_table[index].useIn(p_index);
		}
		
		virtual void generateASM() const override
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({})[({})]", lval, index);
		}
	};
	
} // namespace fl
