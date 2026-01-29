#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class NOOP : public Instruction
	{
	private:
	
	public:

		NOOP() = default;
		~NOOP() = default;

		constexpr virtual size_t size() const override
		{
			return 0;
		}

		std::string generate() const
		{
			return std::string{};
		}

	};
	
} // namespace fl::ins
