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

		// size_t mapOperatorsToTAC(const size_t l, const size_t r, TACTable& tac_table) const
		// {
		// 	switch (op)
		// 	{
		// 	// ASSIGN too complicated to change with this grammar
		// 	// case Operator::ASSIGN:
		// 	// 	return tac_table.add<tac::Assign>(l, r, p_owner);
		// 	case Operator::INDEX:
		// 		return tac_table.add<tac::Index>(l, r, p_owner, left->identifier);
		// 	default:
		// 		panic("internal compier error: no available mapping from operator '{}' to TAC table", op);
		// 		return 0;
		// 	}

		// 	// unreachable, just for g++ to shut up
		// 	return 0;
		// }

	};

} // namespace fl