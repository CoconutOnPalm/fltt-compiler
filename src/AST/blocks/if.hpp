#pragma once

#include <memory>
#include <print>

#include "condition.hpp"
#include "block.hpp"
#include "../ASTNode.hpp"
#include "value.hpp"
#include "../../TAC/codes/label.hpp"


namespace fl::ast
{

	class If : public ASTNode
	{
	private:

		const std::shared_ptr<Condition> condition;
		const std::shared_ptr<Block> block;

	public:
	
		If(Condition* cond_ptr, Block* block_ptr)
			: condition(cond_ptr), block(block_ptr)
		{}

		virtual ~If() = default;

		size_t generateTAC(TACTable& tac_table) const override
		{
			std::shared_ptr<uint64_t> label_id = std::make_shared<uint64_t>(0);
			
			size_t cond = condition->generateTAC(tac_table);
			size_t jmp = generateJump(cond, condition->op, label_id, tac_table);
			block->generateTAC(tac_table);
			return tac_table.add<tac::Label>("endif", label_id);
		}

		std::string __debug_string() const override
		{
			return std::format("if ({})", condition->__debug_string());
		}
	
	private:

		size_t generateJump(const size_t cond, const CondOp op, std::shared_ptr<uint64_t> label_id, TACTable& tac_table) const
		{
			switch (op)
			{
			case CondOp::EQ:
				return tac_table.add<tac::JZ>(cond, label_id);
			case CondOp::NEQ:
				return tac_table.add<tac::JNZ>(cond, label_id);
			case CondOp::GT:
				return tac_table.add<tac::JNZ>(cond, label_id);
			case CondOp::LT:
				return tac_table.add<tac::JNZ>(cond, label_id);
			case CondOp::GEQ:
				return tac_table.add<tac::JZ>(cond, label_id);
			case CondOp::LEQ:
				return tac_table.add<tac::JZ>(cond, label_id);
			default:
				panic("internal compiler error: if AST - operator '{}' not handled", op);
				return 0; // disable warning
			}
		}

	};

} // namespace fl