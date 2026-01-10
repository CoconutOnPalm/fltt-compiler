#pragma once

#include <cstddef>
#include <format>

#include "../../utils/panic.hpp"

namespace fl
{

	enum class REG : size_t
	{
		RA = 0,
		RB = 1,
		RC = 2,
		RD = 3,
		RE = 4,
		RF = 5,
		RG = 6,
		RH = 7,
	};

	constexpr size_t __reg_count = 8;

} // namespace fl


template <typename char_T>
struct std::formatter<fl::REG, char_T> : std::formatter<std::string>
{
	auto format(const fl::REG reg, std::format_context& ctx) const
	{
		std::string reg_str;

		switch (reg)
		{
		case fl::REG::RA:
			reg_str = "RA";
			break;
		case fl::REG::RB:
			reg_str = "RB";
			break;
		case fl::REG::RC:
			reg_str = "RC";
			break;
		case fl::REG::RD:
			reg_str = "RD";
			break;
		case fl::REG::RE:
			reg_str = "RE";
			break;
		case fl::REG::RF:
			reg_str = "RF";
			break;
		case fl::REG::RG:
			reg_str = "RG";
			break;
		case fl::REG::RH:
			reg_str = "RH";
			break;
		default:
			panic("internal compiler error: no formatter defined for register ({})", static_cast<size_t>(reg));
		}
		
		return std::formatter<std::string>::format(reg_str, ctx);
	}
};