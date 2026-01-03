#pragma once

#include "../ASTNode.hpp"


namespace fl
{
	
	class Read : public ASTNode
	{
	private:

	public:

		Read() = default;
		~Read() = default;

		void generateTAC() const 
		{
			std::println("READ");
		}

		std::string __debug_string() const 
		{
			return "READ";
		}
	};

	class Write : public ASTNode
	{
	private:

	public:

		Write() = default;
		~Write() = default;

		void generateTAC() const 
		{
			std::println("WRITE");
		}

		std::string __debug_string() const 
		{
			return "WRITE";
		}
	};

} // namespace fl
