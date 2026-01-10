#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class RST : public Instruction
	{
	private:

		const REG reg;
	
	public:

		RST(const REG _reg)
			: reg(_reg)
		{}

		~RST() = default;

		std::string generate() const
		{
			return std::format("RST {}", reg);
		}

	};
	
} // namespace fl::ins
