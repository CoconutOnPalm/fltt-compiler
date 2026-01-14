#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class MOVE : public Instruction
	{
	private:

		const REG reg;
	
	public:

		MOVE(const REG _reg)
			: reg(_reg)
		{}

		~MOVE() = default;

		std::string generate() const
		{
			return std::format("RST a\nADD {}", reg);
		}

	};
	
} // namespace fl::ins
