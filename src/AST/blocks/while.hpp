#pragma once

#include <memory>
#include <print>

#include "condition.hpp"
#include "block.hpp"
#include "../ASTNode.hpp"
#include "value.hpp"


namespace fl
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

		void generateTAC() const
		{
			condition->generateTAC();
			std::println("while (not {}) JMP <endwhile>", condition->__debug_string());
			block->generateTAC();
		}

		std::string __debug_string() const override
		{
			return std::format("while ({})", condition->__debug_string());
		}
	
	private:

	};

} // namespace fl