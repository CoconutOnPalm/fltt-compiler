#pragma once

#include <memory>
#include <print>

#include "operators.hpp"
#include "../ASTNode.hpp"
#include "value.hpp"


namespace fl
{

	class Condition : public ASTNode
	{
	private:

		const CondOp op;
		const std::shared_ptr<ASTNode> left;
		const std::shared_ptr<ASTNode> right;

	public:
	
		Condition(const CondOp op, ASTNode* left, ASTNode* right)
			: op(op), left(std::shared_ptr<ASTNode>(left)), right(std::shared_ptr<ASTNode>(right))
		{}

		virtual ~Condition() = default;

		void generateTAC() const
		{
			std::println("({:2}, {}, {})", op, left->__debug_string(), right->__debug_string());
		}

		std::string __debug_string() const override
		{
			return std::format("({:2}, {}, {})", op, left->__debug_string(), right->__debug_string());
		}
	
	private:

	};

} // namespace fl