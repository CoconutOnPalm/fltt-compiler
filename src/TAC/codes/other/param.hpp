#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Param : public TAC
	{

		uint64_t param {0};
		const std::string dest;

	public:

		Param(const uint64_t param, const std::string_view destination)
			: param(param), dest(destination)
		{}

		virtual ~Param() = default;

		
		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::PARAM);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[param].useIn(p_index);
		}
		
		virtual void generateASM() const 
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const
		{
			return std::format("param ({}) => {}", param, dest);
		}
	};
	
} // namespace fl
