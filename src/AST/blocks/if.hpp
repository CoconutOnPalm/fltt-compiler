#pragma once

#include <memory>
#include <print>

#include "condition.hpp"
#include "block.hpp"
#include "../ASTNode.hpp"
#include "value.hpp"
#include "../../TAC/codes/other/label.hpp"


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

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {block};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			std::shared_ptr<uint64_t> label_id = std::make_shared<uint64_t>(0);

			condition->invert();
			
			size_t cond = condition->generateTAC(tac_table);
			size_t jmp = generateJumpIfTrue(cond, condition->getOperator(), label_id, tac_table);
			block->generateTAC(tac_table);
			return tac_table.add<tac::Label>("endif", label_id);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("if ({})", condition->__debug_string());
		}
	
	private:

	};

} // namespace fl