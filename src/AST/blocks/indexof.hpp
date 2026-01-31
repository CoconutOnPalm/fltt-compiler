#pragma once

#include <memory>
#include <print>
#include <functional>

#include "../ASTNode.hpp"
#include "operators.hpp"
#include "../../utils/panic.hpp"
#include "number.hpp"
#include "identifier.hpp"

#include "../../TAC/codes/expressions/assign.hpp"
#include "../../TAC/codes/expressions/add.hpp"
#include "../../TAC/codes/expressions/sub.hpp"
#include "../../TAC/codes/expressions/mult.hpp"
#include "../../TAC/codes/expressions/div.hpp"
#include "../../TAC/codes/expressions/mod.hpp"
#include "../../TAC/codes/expressions/index.hpp"


namespace fl::ast
{

	class IndexOf : public Identifier
	{
	private:

		const std::shared_ptr<ASTNode> index;

	public:
	
		IndexOf(const std::string_view id, ASTNode* ind)
			: Identifier(id), index(std::shared_ptr<ASTNode>(ind))
		{}

		virtual ~IndexOf() = default;

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {index};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			size_t ridx = index->generateTAC(tac_table);
			return tac_table.add<tac::Index>(identifier, ridx, p_owner);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("([], {}, {})", identifier, index->__debug_string());
		}
	
	private:

	};

} // namespace fl