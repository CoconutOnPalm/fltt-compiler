#pragma once

#include "../ASTNode.hpp"


namespace fl::ast
{
	
	class Read : public ASTNode
	{
	private:

	public:

		Read() = default;
		~Read() = default;

		size_t generateTAC(TACTable& tac_table) const override
		{
			std::println("READ");
			return 0;
		}

		std::string __debug_string() const override
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

		size_t generateTAC(TACTable& tac_table) const override
		{
			std::println("WRITE");
			return 0;
		}

		std::string __debug_string() const override
		{
			return "WRITE";
		}
	};

} // namespace fl
