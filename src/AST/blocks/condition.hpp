#pragma once

#include <memory>
#include <print>

#include "operators.hpp"
#include "../ASTNode.hpp"
#include "value.hpp"

#include "../../TAC/codes/comparisons/eq.hpp"
#include "../../TAC/codes/comparisons/neq.hpp"
#include "../../TAC/codes/comparisons/gt.hpp"
#include "../../TAC/codes/comparisons/lt.hpp"
#include "../../TAC/codes/comparisons/geq.hpp"
#include "../../TAC/codes/comparisons/leq.hpp"

#include "../../TAC/codes/jump/jz.hpp"
#include "../../TAC/codes/jump/jnz.hpp"


namespace fl::ast
{

	class Condition : public ASTNode
	{
	public:

		const CondOp op;
		
	private:

		const std::shared_ptr<ASTNode> left;
		const std::shared_ptr<ASTNode> right;

	public:
	
		Condition(const CondOp op, ASTNode* left, ASTNode* right)
			: op(op), left(std::shared_ptr<ASTNode>(left)), right(std::shared_ptr<ASTNode>(right))
		{}

		virtual ~Condition() = default;

		size_t generateTAC(TACTable& tac_table) const override
		{
			size_t lidx = left->generateTAC(tac_table);
			size_t ridx = right->generateTAC(tac_table);

			return mapOperatorsToTAC(op, lidx, ridx, tac_table);
		}

		std::string __debug_string() const override
		{
			return std::format("({:2}, {}, {})", op, left->__debug_string(), right->__debug_string());
		}
	
	private:

		static CondOp invert(const CondOp _operator)
		{
			switch (_operator)
			{
			case CondOp::EQ:
				return CondOp::NEQ;
			case CondOp::NEQ:
				return CondOp::EQ;
			case CondOp::GT:
				return CondOp::LEQ;
			case CondOp::LT:
				return CondOp::GEQ;
			case CondOp::GEQ:
				return CondOp::LT;
			case CondOp::LEQ:
				return CondOp::GT;
			default:
				break;
			}
		}

		size_t mapOperatorsToTAC(const CondOp _operator, const size_t l, const size_t r, TACTable& tac_table) const
		{
			switch (_operator)
			{
			case CondOp::EQ:
				return tac_table.add<tac::Equal>(l, r);
				// size_t a = tac_table.add<tac::Sub>(l, r);
				// size_t b = tac_table.add<tac::Sub>(r, l);
				// size_t cond = tac_table.add<tac::Add>(a, b);

			case CondOp::NEQ:
				return tac_table.add<tac::NotEqual>(l, r);
				// size_t a = tac_table.add<tac::Sub>(l, r);
				// size_t b = tac_table.add<tac::Sub>(r, l);
				// size_t cond = tac_table.add<tac::Add>(a, b);
			case CondOp::GT:
				return tac_table.add<tac::GreaterThan>(l, r);
				// size_t cond = tac_table.add<tac::Sub>(r, l);
				// return tac_table.add<tac::JZ>(cond);
			case CondOp::LT:
				return tac_table.add<tac::LessThan>(l, r);
			case CondOp::GEQ:
				return tac_table.add<tac::GreaterOrEqual>(l, r);
			case CondOp::LEQ:
				return tac_table.add<tac::LessOrEqual>(l, r);
			default:
				panic("internal compiler error: conditional operator '{}' not mapped", _operator);
				return 0; // disable warning
			}
		}

	};

} // namespace fl