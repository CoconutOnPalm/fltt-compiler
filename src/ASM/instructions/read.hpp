#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class READ : public Instruction
	{
	private:

	public:

		READ()
		{}

		~READ() = default;

		std::string generate() const
		{
			return std::format("READ");
		}

	};
	
} // namespace fl::ins
