#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class JZERO : public Instruction
	{
	private:

		const size_t pos;

	public:

		JZERO(const size_t _pos)
			: pos(_pos)
		{}

		~JZERO() = default;

		std::string generate() const
		{
			return std::format("JZERO {}", pos);
		}

	};
	
} // namespace fl::ins
