#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class JUMP : public Instruction
	{
	private:

		const size_t pos;

	public:

		JUMP(const size_t _pos)
			: pos(_pos)
		{}

		~JUMP() = default;

		std::string generate() const
		{
			return std::format("JUMP {}", pos);
		}

	};
	
} // namespace fl::ins
