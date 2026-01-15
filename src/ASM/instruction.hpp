#pragma once

#include <string>
#include <unordered_map>
#include <format>


namespace fl
{

	class Instruction
	{
	public:

		Instruction() = default;
		virtual ~Instruction() = default;

		virtual std::string generate() const = 0;

		// how many lines of instruction does this class generate
		constexpr virtual size_t size() const 
		{ return 1; }

		virtual void updateJumpDestination(const std::unordered_map<size_t, size_t> label_map)
		{ /* do nothing */ }
	};
	
} // namespace fl
