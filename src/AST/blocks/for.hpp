#pragma once

#include <memory>


#include "../ASTNode.hpp"
#include "value.hpp"
#include "block.hpp"


namespace fl
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

		void generateTAC() const 
		{
			iterator->generateTAC();
			std::print(" = ");
			from->generateTAC();
			std::println("{}", __debug_string());
			block->generateTAC();
		}

		std::string __debug_string() const override
		{
			return std::format("for ({} : [{}..{}])", iterator->__debug_string(), from->__debug_string(), to->__debug_string());
		}

	};
	
} // namespace fl
