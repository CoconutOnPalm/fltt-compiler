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
			// else if (dynamic_cast<Number*>(left.get()) && dynamic_cast<Identifier*>(right.get()) && !dynamic_cast<IndexOf*>(right.get()))
			// {
			// 	// 'a' is an immediate, 'b' is an identifier
			// 	uint64_t a = dynamic_cast<Number*>(left.get())->val;
			// 	std::string b = dynamic_cast<Identifier*>(right.get())->identifier;
			// 	if (a <= 4)
			// 	{
			// 		size_t last_tac = 0;
			// 		for (uint64_t i = 0; i < a; i++)
			// 			last_tac = tac_table.add<tac::Inc>(b, p_owner);
			// 		return last_tac;
			// 	}
			// }
			// else if (dynamic_cast<Identifier*>(left.get()) && dynamic_cast<Number*>(right.get()) && !dynamic_cast<IndexOf*>(left.get()))
			// {
			// 	// 'a' is an identifier, 'b' is an immediate
			// 	std::string a = dynamic_cast<Identifier*>(left.get())->identifier;
			// 	uint64_t b = dynamic_cast<Number*>(right.get())->val;
			// 	if (b <= 4)
			// 	{
			// 		size_t last_tac = 0;
			// 		for (uint64_t i = 0; i < b; i++)
			// 			last_tac = tac_table.add<tac::Inc>(a, p_owner);
			// 		return last_tac;
			// 	}
			// }
			
			return defaultAddition(tac_table);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("( +, {}, {})", left->__debug_string(), right->__debug_string());
		}
	
	private:

		size_t defaultAddition(TACTable& tac_table) const
		{
			size_t lidx = left->generateTAC(tac_table);
			size_t ridx = right->generateTAC(tac_table);
			
			return tac_table.add<tac::Add>(lidx, ridx, p_owner);
		}

	};

} // namespace fl