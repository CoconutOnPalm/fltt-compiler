#pragma once

#include <cstdint>
#include <string>
#include <format>


namespace fl
{

	class Number : public ASTNode
	{
	private:

		const uint64_t val;

	public:

		Number(const uint64_t val) : val(val) {}

		void generateTAC() const { std::println("({})", val); }

		std::string __debug_string() const override
		{
			return std::format("{}", val);
		}
	};

	struct Identifier : public ASTNode
	{
	private:

		const std::string identifier;

	public:

		Identifier(const std::string& id) : identifier(id) {}

		void generateTAC() const { std::println("{}", identifier); }

		std::string __debug_string() const override
		{
			return identifier;
		}
	};
	
} // namespace fl
