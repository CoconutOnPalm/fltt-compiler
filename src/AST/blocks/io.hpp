#pragma once

#include "../ASTNode.hpp"

#include "../../TAC/codes/other/read.hpp"
#include "../../TAC/codes/other/write.hpp"


namespace fl::ast
{
	
	class Read : public ASTNode
	{
	private:

		std::shared_ptr<ASTNode> identifier;

	public:

		Read(ASTNode* id)
			: identifier(id)
		{}

		~Read() = default;

		std::vector<std::shared_ptr<ASTNode>> getChildren()
		{
			return {identifier};
		}

		size_t generateTAC(TACTable& tac_table) const override
		{
			size_t var = identifier->generateTAC(tac_table);
			return tac_table.add<tac::Read>(var);
		}

		std::string __debug_string() const override
		{
			return "READ";
		}
	};

	class Write : public ASTNode
	{
	private:

		std::shared_ptr<ASTNode> identifier;

	public:

		Write(ASTNode* id)
			: identifier(id)
		{}

		~Write() = default;

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {identifier};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			size_t var = identifier->generateTAC(tac_table);
			return tac_table.add<tac::Write>(var);
		}

		virtual std::string __debug_string() const override
		{
			return "WRITE";
		}
	};

} // namespace fl
