#pragma once

#include <memory>
#include <print>
#include <functional>

#include "../../ASTNode.hpp"
#include "../operators.hpp"
#include "../../../utils/panic.hpp"
#include "../number.hpp"
#include "../identifier.hpp"

#include "../../../TAC/codes/expressions/div.hpp"
#include "../../../TAC/codes/special/log2.hpp"


namespace fl::ast
{

	class Division : public ASTNode
	{
	private:

		const std::shared_ptr<ASTNode> left;
		const std::shared_ptr<ASTNode> right;

	public:
	
		Division(ASTNode* left, ASTNode* right)
			: left(std::shared_ptr<ASTNode>(left)), right(std::shared_ptr<ASTNode>(right))
		{}

		virtual ~Division() = default;

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

				if (a == 0 || b == 0)
					return tac_table.add<tac::LDI>(0, p_owner);
				else
					return tac_table.add<tac::LDI>(a / b, p_owner);
			}
			else if (dynamic_cast<Identifier*>(left.get()) && dynamic_cast<Number*>(right.get()))
			{
				// 'a' is an identifier, 'b' is an immediate
				uint64_t b = dynamic_cast<Number*>(right.get())->val;
				if (std::bitset<64>(b).count() == 1) // is a power of 2
				{
					size_t lidx = left->generateTAC(tac_table);
					size_t exponent = std::bit_width(b) - 1;
					return tac_table.add<tac::Log2>(lidx, exponent, p_owner);
				}
			}
			
			size_t lidx = left->generateTAC(tac_table);
			size_t ridx = right->generateTAC(tac_table);
			
			return tac_table.add<tac::Div>(lidx, ridx, p_owner);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("( +, {}, {})", left->__debug_string(), right->__debug_string());
		}
	
	private:

	};

} // namespace fl