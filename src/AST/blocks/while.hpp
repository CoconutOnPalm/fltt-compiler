#pragma once

#include <memory>
#include <print>

#include "condition.hpp"
#include "block.hpp"
#include "../ASTNode.hpp"
#include "value.hpp"


namespace fl::ast
{

	class While : public ASTNode
	{
	private:

		const std::shared_ptr<Condition> condition;
		const std::shared_ptr<Block> block;

	public:
	
		While(Condition* cond_ptr, Block* block_ptr)
			: condition(cond_ptr), block(block_ptr)
		{}

		virtual ~While() = default;

		size_t generateTAC(TACTable& tac_table) const override
		{
			condition->generateTAC(tac_table);
			std::println("while (not {}) JMP <endwhile>", condition->__debug_string());
			block->generateTAC(tac_table);
			return 0;
		}

		std::string __debug_string() const override
		{
			return std::format("while ({})", condition->__debug_string());
		}
	
	private:

	};

} // namespace fl