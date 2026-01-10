#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class ADD : public Instruction
	{
	private:

		const REG reg;
	
	public:

		ADD(const REG _reg)
			: reg(_reg)
		{}

		~ADD() = default;

		std::string generate() const
		{
			return std::format("ADD {}", reg);
		}

	};
	
} // namespace fl::ins
