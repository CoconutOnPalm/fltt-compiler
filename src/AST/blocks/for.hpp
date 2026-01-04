#pragma once

#include <memory>


#include "../ASTNode.hpp"
#include "value.hpp"
#include "block.hpp"


namespace fl::ast
{

	class For : public ASTNode
	{
	private:

		std::shared_ptr<Identifier> iterator;
		std::shared_ptr<ASTNode> from;
		std::shared_ptr<ASTNode> to;
		std::shared_ptr<Block> block;

		const int step; // -1 / 1
	
	public:

		For(Identifier* id_ptr, ASTNode* from, ASTNode* to, Block* block, const int step)
			: iterator(id_ptr), from(from), to(to), block(block), step(step)
		{}

		virtual ~For() = default;

		size_t generateTAC(TACTable& tac_table) const override
		{
			iterator->generateTAC(tac_table);
			std::print(" = ");
			from->generateTAC(tac_table);
			std::println("{}", __debug_string());
			block->generateTAC(tac_table);
			return 0;
		}

		std::string __debug_string() const override
		{
			return std::format("for ({} : [{}..{}])", iterator->__debug_string(), from->__debug_string(), to->__debug_string());
		}

	};
	
} // namespace fl
