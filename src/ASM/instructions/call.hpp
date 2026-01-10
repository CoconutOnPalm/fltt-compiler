#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class CALL : public Instruction
	{
	private:

		const size_t pos;

	public:

		CALL(const size_t _pos)
			: pos(_pos)
		{}

		~CALL() = default;

		std::string generate() const
		{
			return std::format("CALL {}", pos);
		}

	};
	
} // namespace fl::ins
