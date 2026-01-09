#pragma once

#include <print>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Mult : public TAC
	{
	private:

		const size_t left_index;
		const size_t right_index;
	
	public:

		Mult(const size_t left, const size_t right, const std::string_view owning_proc)
			: TAC(owning_proc), left_index(left), right_index(right)
		{}

		virtual ~Mult() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::EXPRESSION, p_owning_procedure);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[left_index].useIn(p_index);
			info_table[right_index].useIn(p_index);
		}
		
		virtual void generateASM() const override
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({}) * ({})", left_index, right_index);
		}
		
	};
	
} // namespace fl
