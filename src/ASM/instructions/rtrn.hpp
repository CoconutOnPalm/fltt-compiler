#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class RTRN : public Instruction
	{
	private:

	public:

		RTRN()
		{}

		~RTRN() = default;

		std::string generate() const
		{
			return std::format("RTRN");
		}

	};
	
} // namespace fl::ins
