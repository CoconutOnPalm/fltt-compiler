#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class JPOS : public Instruction
	{
	private:

		const size_t pos;

	public:

		JPOS(const size_t _pos)
			: pos(_pos)
		{}

		~JPOS() = default;

		std::string generate() const
		{
			return std::format("JPOS {}", pos);
		}

	};
	
} // namespace fl::ins
