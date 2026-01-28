#pragma once

#include <memory>


#include "../ASTNode.hpp"
#include "number.hpp"
#include "identifier.hpp"
#include "block.hpp"

#include "../../TAC/codes/other/iterator_inc.hpp"
#include "../../TAC/codes/other/iterator_dec.hpp"
#include "../../TAC/codes/other/LDC.hpp"

namespace fl::ast
{

	class For : public ASTNode
	{
	private:

		enum class Step
		{
			TO,
			DOWNTO,
		};

		std::shared_ptr<Identifier> iterator;
		std::shared_ptr<ASTNode> from;
		std::shared_ptr<ASTNode> to;
		std::shared_ptr<Block> block;

		const Step step;

	public:

		For(Identifier* id_ptr, ASTNode* from, ASTNode* to, Block* block, const int _step)
			: iterator(id_ptr), from(from), to(to), block(block), step(_step > 0 ? Step::TO : Step::DOWNTO)
		{}

		virtual ~For() = default;

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return { iterator, from, to, block };
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			std::shared_ptr<uint64_t> begin_for = std::make_shared<uint64_t>(0);
			std::shared_ptr<uint64_t> end_for = std::make_shared<uint64_t>(0);
			std::shared_ptr<uint64_t> exit_for = std::make_shared<uint64_t>(0);

			// for header
			// size_t it =  iterator->generateTAC(tac_table);
			size_t it = tac_table.add<tac::LDC>(iterator->identifier, p_owner);
			size_t beg = from->generateTAC(tac_table);
			// size_t end = to->generateTAC(tac_table);
			tac_table.add<tac::ForceAssign>(it, beg, p_owner);

			if (step == Step::DOWNTO)
			{
				// to prevent "for i FROM 0 DOWNTO x DO" (where x > 0) from generating block on exit
				it = tac_table.add<tac::LDC>(iterator->identifier, p_owner);
				size_t end = to->generateTAC(tac_table);
				size_t jmp_cond = tac_table.add<tac::LessThan>(it, end, p_owner);
				generateJumpIfTrue(jmp_cond, CondOp::LT, exit_for, tac_table, p_owner);
			}

			// for
			tac_table.add<tac::Label>("for", begin_for, p_owner);
			
			it = tac_table.add<tac::LDC>(iterator->identifier, p_owner);
			size_t end = to->generateTAC(tac_table);
			if (step == Step::TO)
			{
				size_t jmp_cond = tac_table.add<tac::GreaterThan>(it, end, p_owner);
				generateJumpIfTrue(jmp_cond, CondOp::GT, end_for, tac_table, p_owner);
			}
			else // step = Step::DOWNTO
			{
				size_t jmp_cond = tac_table.add<tac::LessOrEqual>(it, end, p_owner);
				generateJumpIfTrue(jmp_cond, CondOp::LEQ, end_for, tac_table, p_owner);
			}

			block->generateTAC(tac_table);

			
			if (step == Step::TO)
				tac_table.add<tac::IterInc>(iterator->identifier, p_owner);
			else
				tac_table.add<tac::IterDec>(iterator->identifier, p_owner);
			
			tac_table.add<tac::JUMP>(begin_for, p_owner);

			// endfor 
			size_t ret = tac_table.add<tac::Label>("endfor", end_for, p_owner);

			if (step == Step::DOWNTO)
			{
				// generate one more pass
				ret = block->generateTAC(tac_table);
			}

			return tac_table.add<tac::Label>("exitfor", exit_for, p_owner);
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
