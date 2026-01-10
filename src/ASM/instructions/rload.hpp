#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{
	
	class RLOAD : public Instruction
	{
	private:

		const REG reg;

	public:

		RLOAD(const REG _reg)
			: reg(_reg)
		{}
		virtual ~RLOAD() = default;

		std::string generate() const
		{
			return std::format("RLOAD {}", reg);
		}
	};
	
} // namespace fl::asm
