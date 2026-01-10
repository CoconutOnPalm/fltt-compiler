#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class DEC : public Instruction
	{
	private:

		const REG reg;
	
	public:

		DEC(const REG _reg)
			: reg(_reg)
		{}

		~DEC() = default;

		std::string generate() const
		{
			return std::format("DEC {}", reg);
		}

	};
	
} // namespace fl::ins
