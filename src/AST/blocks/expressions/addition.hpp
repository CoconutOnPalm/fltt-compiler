#pragma once

#include <memory>
#include <print>
#include <functional>

#include "../../ASTNode.hpp"
#include "../operators.hpp"
#include "../../../utils/panic.hpp"
#include "../number.hpp"
#include "../identifier.hpp"
#include "../indexof.hpp"

#include "../../../TAC/codes/expressions/add.hpp"
#include "../../../TAC/codes/special/small_add.hpp"


namespace fl::ast
{

	class Addition : public ASTNode
	{
	private:

		const std::shared_ptr<ASTNode> left;
		const std::shared_ptr<ASTNode> right;

	public:
	
		Addition(ASTNode* left, ASTNode* right)
			: left(std::shared_ptr<ASTNode>(left)), right(std::shared_ptr<ASTNode>(right))
		{}

		virtual ~Addition() = default;

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {left, right};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			if (dynamic_cast<Number*>(left.get()) && dynamic_cast<Number*>(right.get()))
			{
				// a and b are immediates
				uint64_t a = dynamic_cast<Number*>(left.get())->val;
				uint64_t b = dynamic_cast<Number*>(right.get())->val;

				if (a <= std::numeric_limits<uint64_t>::max() - b) // a + b will overflow
					return tac_table.add<tac::LDI>(a + b, p_owner);
			}
			// too much overhead cost
			// else if (dynamic_cast<Number*>(left.get()) && dynamic_cast<Identifier*>(right.get()))
			// {
			// 	// 'a' is an immediate, 'b' is an identifier
			// 	uint64_t a = dynamic_cast<Number*>(left.get())->val;
			// 	if (a <= 3) // is 'small'
			// 	{
			// 		size_t ridx = right->generateTAC(tac_table);
			// 		return tac_table.add<tac::SmallAdd>(ridx, a, p_owner);
			// 	}

			// }
			// else if (dynamic_cast<Identifier*>(left.get()) && dynamic_cast<Number*>(right.get()))
			// {
			// 	// 'a' is an identifier, 'b' is an immediate
			// 	uint64_t b = dynamic_cast<Number*>(right.get())->val;
			// 	if (b <= 3) // is 'small'
			// 	{
			// 		size_t lidx = left->generateTAC(tac_table);
			// 		return tac_table.add<tac::SmallAdd>(lidx, b, p_owner);
			// 	}
			// }


			size_t lidx = left->generateTAC(tac_table);
			size_t ridx = right->generateTAC(tac_table);
			
			return tac_table.add<tac::Add>(lidx, ridx, p_owner);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("( +, {}, {})", left->__debug_string(), right->__debug_string());
		}
	
	};

} // namespace fl