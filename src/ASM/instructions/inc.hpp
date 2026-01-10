#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class INC : public Instruction
	{
	private:

		const REG reg;
	
	public:

		INC(const REG _reg)
			: reg(_reg)
		{}

		~INC() = default;

		std::string generate() const
		{
			return std::format("INC {}", reg);
		}

	};
	
} // namespace fl::ins
