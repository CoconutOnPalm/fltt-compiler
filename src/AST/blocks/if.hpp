#pragma once

#include <memory>
#include <print>

#include "condition.hpp"
#include "block.hpp"
#include "../ASTNode.hpp"
#include "value.hpp"


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
			condition->generateTAC(tac_table);
			std::println("if (not {}) JMP <endif>", condition->__debug_string());
			block->generateTAC(tac_table);
			return 0;
		}

		std::string __debug_string() const override
		{
			return std::format("if ({})", condition->__debug_string());
		}
	
	private:

	};

} // namespace fl