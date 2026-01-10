#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class SWP : public Instruction
	{
	private:

		const REG reg;
	
	public:

		SWP(const REG _reg)
			: reg(_reg)
		{}

		~SWP() = default;

		std::string generate() const
		{
			return std::format("SWP {}", reg);
		}

	};
	
} // namespace fl::ins
