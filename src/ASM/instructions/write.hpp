#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class WRITE : public Instruction
	{
	private:

	public:

		WRITE()
		{}

		~WRITE() = default;

		std::string generate() const
		{
			return std::format("WRITE");
		}

	};
	
} // namespace fl::ins
