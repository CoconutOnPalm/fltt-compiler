#pragma once

#include <memory>


#include "../ASTNode.hpp"
#include "number.hpp"
#include "identifier.hpp"
#include "block.hpp"

#include "../../TAC/codes/other/inc.hpp"
#include "../../TAC/codes/other/dec.hpp"
#include "../../TAC/codes/other/LDC.hpp"

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

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return { iterator, from, to, block };
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			std::shared_ptr<uint64_t> end_for = std::make_shared<uint64_t>(0);
			std::shared_ptr<uint64_t> begin_for = std::make_shared<uint64_t>(0);

			// for header
			// size_t it =  iterator->generateTAC(tac_table);
			size_t it = tac_table.add<tac::LDC>(iterator->identifier, p_owner);
			size_t beg = from->generateTAC(tac_table);
			size_t end = to->generateTAC(tac_table);
			tac_table.add<tac::ForceAssign>(it, beg, p_owner);

			// for
			tac_table.add<tac::Label>("for", begin_for, p_owner);
			size_t jmp_cond = tac_table.add<tac::Equal>(it, end, p_owner);

			if (step == 1)
			{
				// use >= as == is more expensive
				generateJumpIfTrue(jmp_cond, CondOp::GEQ, end_for, tac_table, p_owner);
				tac_table.add<tac::Inc>(it, p_owner);
			}
			else // step = -1 
			{
				generateJumpIfTrue(jmp_cond, CondOp::LEQ, end_for, tac_table, p_owner);
				tac_table.add<tac::Dec>(it, p_owner);
			}

			block->generateTAC(tac_table);

			tac_table.add<tac::JUMP>(begin_for, p_owner);

			// endfor 
			return tac_table.add<tac::Label>("endfor", end_for, p_owner);
		}

		virtual void declareInBlock(SymbolTable& symbol_table) override
		{
			// mark variable as const
			symbol_table.add<Variable>(iterator->identifier, SymbolType::IN);

			auto children = getChildren();
			for (size_t i = 0; const auto& child_ptr : children)
			{
				child_ptr->declareInBlock(symbol_table);
				i++;
			}
		}

		virtual std::string __debug_string() const override
		{
			return std::format("for ({} : [{}..{}])", iterator->__debug_string(), from->__debug_string(), to->__debug_string());
		}

	};

} // namespace fl
