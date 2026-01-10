#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class HALT : public Instruction
	{
	private:

	public:

		HALT()
		{}

		~HALT() = default;

		std::string generate() const
		{
			return std::format("HALT");
		}

	};
	
} // namespace fl::ins
