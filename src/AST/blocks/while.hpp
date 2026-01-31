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

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {condition, block};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			std::shared_ptr<uint64_t> while_id = std::make_shared<uint64_t>(0);
			std::shared_ptr<uint64_t> endwhile_id = std::make_shared<uint64_t>(0);
			
			condition->invert();
			
			size_t while_begin = 	tac_table.add<tac::Label>("while", while_id, p_owner);
			size_t cond = 			condition->generateTAC(tac_table);
			size_t endwhile_jmp = 	generateJumpIfTrue(cond, condition->getOperator(), endwhile_id, tac_table, p_owner);
			size_t body = 			block->generateTAC(tac_table);
			size_t while_jmp = 		tac_table.add<tac::JUMP>(while_id, p_owner);

			return 					tac_table.add<tac::Label>("endwhile", endwhile_id, p_owner);
			
			condition->generateTAC(tac_table);
			block->generateTAC(tac_table);
			return 0;
		}

		virtual std::string __debug_string() const override
		{
			return std::format("while ({})", condition->__debug_string());
		}
	
	private:

	};

} // namespace fl