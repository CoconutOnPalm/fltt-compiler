#pragma once

#include <memory>
#include <print>

#include "condition.hpp"
#include "block.hpp"
#include "../ASTNode.hpp"
#include "value.hpp"


namespace fl
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

		void generateTAC() const
		{
			condition->generateTAC();
			std::println("if (not {}) JMP <else>", condition->__debug_string());
			if_block->generateTAC();
			std::println("JMP <endif>");
			std::println("else");
			else_block->generateTAC();
		}

		std::string __debug_string() const override
		{
			return std::format("if ({})", condition->__debug_string());
		}
	
	private:

	};

} // namespace fl