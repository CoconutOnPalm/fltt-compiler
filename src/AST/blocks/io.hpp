#pragma once

#include "../ASTNode.hpp"

#include "../../TAC/codes/other/read.hpp"
#include "../../TAC/codes/other/write.hpp"


namespace fl::ast
{
	
	class Read : public ASTNode
	{
	private:

	public:

		Read() = default;
		~Read() = default;

		std::vector<std::shared_ptr<ASTNode>> getChildren()
		{
			return {};
		}

		size_t generateTAC(TACTable& tac_table) const override
		{
			return tac_table.add<tac::Read>();
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

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			return tac_table.add<tac::Write>();
		}

		virtual std::string __debug_string() const override
		{
			return "WRITE";
		}
	};

} // namespace fl
