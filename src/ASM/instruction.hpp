#pragma once

#include <string>
#include <format>


namespace fl
{

	class Instruction
	{
	public:

		Instruction() = default;
		virtual ~Instruction() = default;

		virtual std::string generate() const = 0;
		
	};
	
} // namespace fl
