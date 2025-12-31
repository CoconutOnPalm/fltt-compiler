#pragma once

#include <memory>
#include <print>

#include "condition.hpp"
#include "block.hpp"
#include "../ASTNode.hpp"
#include "value.hpp"


namespace fl
{

	class DoWhile : public ASTNode
	{
	private:

		const std::shared_ptr<Condition> condition;
		const std::shared_ptr<Block> block;

	public:
	
		DoWhile(Condition* cond_ptr, Block* block_ptr)
			: condition(cond_ptr), block(block_ptr)
		{}

		virtual ~DoWhile() = default;

		void generateTAC() const
		{
			std::println("repeat");
			block->generateTAC();
			condition->generateTAC();
			std::println("until (not {}) JMP <repeat>", condition->__debug_string());
		}

		std::string __debug_string() const override
		{
			return std::format("repeat {{ }} until ({})", condition->__debug_string());
		}
	
	private:

	};

} // namespace fl