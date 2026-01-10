#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class SUB : public Instruction
	{
	private:

		const REG reg;
	
	public:

		SUB(const REG _reg)
			: reg(_reg)
		{}

		~SUB() = default;

		std::string generate() const
		{
			return std::format("SUB {}", reg);
		}

	};
	
} // namespace fl::ins
