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
			condition->generateTAC(tac_table);
			std::println("if (not {}) JMP <else>", condition->__debug_string());
			if_block->generateTAC(tac_table);
			std::println("JMP <endif>");
			std::println("else");
			else_block->generateTAC(tac_table);
			return 0;
		}

		std::string __debug_string() const override
		{
			return std::format("if ({})", condition->__debug_string());
		}
	
	private:

	};

} // namespace fl