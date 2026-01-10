#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class SHL : public Instruction
	{
	private:

		const REG reg;
	
	public:

		SHL(const REG _reg)
			: reg(_reg)
		{}

		~SHL() = default;

		std::string generate() const
		{
			return std::format("SHL {}", reg);
		}

	};
	
} // namespace fl::ins
