#pragma once

#include <cstddef>


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
