#pragma once

#include <memory>
#include <print>
#include <functional>

#include "../ASTNode.hpp"
#include "operators.hpp"
#include "../../utils/panic.hpp"
#include "value.hpp"

#include "../../TAC/codes/expressions/assign.hpp"
#include "../../TAC/codes/expressions/add.hpp"
#include "../../TAC/codes/expressions/sub.hpp"
#include "../../TAC/codes/expressions/mult.hpp"
#include "../../TAC/codes/expressions/div.hpp"
#include "../../TAC/codes/expressions/mod.hpp"
#include "../../TAC/codes/expressions/index.hpp"


namespace fl::ast
{

	class Expression : public ASTNode
	{
	private:

		const Operator op;
		const std::shared_ptr<ASTNode> left;
		const std::shared_ptr<ASTNode> right;

	public:
	
		Expression(const Operator op, ASTNode* left, ASTNode* right)
			: op(op), left(std::shared_ptr<ASTNode>(left)), right(std::shared_ptr<ASTNode>(right))
		{}

		virtual ~Expression() = default;

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {left, right};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			size_t lidx = left->generateTAC(tac_table);
			size_t ridx = right->generateTAC(tac_table);
			
			return mapOperatorsToTAC(lidx, ridx, tac_table);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({:2}, {}, {})", op, left->__debug_string(), right->__debug_string());
		}
	
	private:

		size_t mapOperatorsToTAC(const size_t l, const size_t r, TACTable& tac_table) const
		{
			switch (op)
			{
			case Operator::ASSIGN:
				return tac_table.add<tac::Assign>(l, r);
			case Operator::ADD:
				return tac_table.add<tac::Add>(l, r);
			case Operator::SUB:
				return tac_table.add<tac::Sub>(l, r);
			case Operator::MULT:
				return tac_table.add<tac::Mult>(l, r);
			case Operator::DIV:
				return tac_table.add<tac::Div>(l, r);
			case Operator::MOD:
				return tac_table.add<tac::Mod>(l, r);
			case Operator::INDEX:
				return tac_table.add<tac::Index>(l, r);
			default:
				panic("internal compier error: no available mapping from operator '{}' to TAC table", op);
				return 0;
			}

			// unreachable, just for g++ to shut up
			return 0;
		}

	};

} // namespace fl