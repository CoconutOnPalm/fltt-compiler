#pragma once

#include <memory>
#include <print>

#include "condition.hpp"
#include "block.hpp"
#include "../ASTNode.hpp"
#include "value.hpp"


namespace fl::ast
{

	class IfElse : public ASTNode
	{
	private:

		const std::shared_ptr<Condition> condition;
		const std::shared_ptr<Block> if_block;
		const std::shared_ptr<Block> else_block;

	public:
	
		IfElse(Condition* cond_ptr, Block* if_block_ptr, Block* else_block_ptr)
			: condition(cond_ptr), if_block(if_block_ptr), else_block(else_block_ptr)
		{}

		virtual ~IfElse() = default;

		size_t generateTAC(TACTable& tac_table) const override
		{
			std::shared_ptr<uint64_t> else_label = std::make_shared<uint64_t>(0);
			std::shared_ptr<uint64_t> endif_label = std::make_shared<uint64_t>(0);

			condition->invert();
			
			// if
			size_t cond = 		condition->generateTAC(tac_table);
			size_t jmp = 		generateJump(cond, condition->getOperator(), else_label, tac_table);
			size_t if_body = 	if_block->generateTAC(tac_table);
			tac_table.add<tac::JUMP>(endif_label);

			// else
			tac_table.add<tac::Label>("else", else_label);
			size_t else_body = 	else_block->generateTAC(tac_table);

			// endif
			return tac_table.add<tac::Label>("endif", endif_label);
		}

		std::string __debug_string() const override
		{
			return std::format("if ({})", condition->__debug_string());
		}
	
	private:

	};

} // namespace fl