#pragma once

#include <memory>
#include <print>

#include "condition.hpp"
#include "block.hpp"
#include "../ASTNode.hpp"
#include "number.hpp"
#include "identifier.hpp"


namespace fl::ast
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

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {block};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			std::shared_ptr<uint64_t> do_label = std::make_shared<uint64_t>(0);

			condition->invert();

			// do
			tac_table.add<tac::Label>("do", do_label, p_owner);
			block->generateTAC(tac_table);

			// while
			size_t cond = condition->generateTAC(tac_table);
			return generateJumpIfTrue(cond, condition->getOperator(), do_label, tac_table, p_owner);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("repeat {{ }} until ({})", condition->__debug_string());
		}
	
	private:

	};

} // namespace fl