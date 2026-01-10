#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class SHR : public Instruction
	{
	private:

		const REG reg;
	
	public:

		SHR(const REG _reg)
			: reg(_reg)
		{}

		~SHR() = default;

		std::string generate() const
		{
			return std::format("SHR {}", reg);
		}

	};
	
} // namespace fl::ins
