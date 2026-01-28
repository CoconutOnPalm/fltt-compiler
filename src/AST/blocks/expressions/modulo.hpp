#pragma once

#include <memory>
#include <print>
#include <functional>

#include "../../ASTNode.hpp"
#include "../operators.hpp"
#include "../../../utils/panic.hpp"
#include "../number.hpp"
#include "../identifier.hpp"

#include "../../../TAC/codes/expressions/mod.hpp"


namespace fl::ast
{

	class Modulo : public ASTNode
	{
	private:

		const std::shared_ptr<ASTNode> left;
		const std::shared_ptr<ASTNode> right;

	public:
	
		Modulo(ASTNode* left, ASTNode* right)
			: left(std::shared_ptr<ASTNode>(left)), right(std::shared_ptr<ASTNode>(right))
		{}

		virtual ~Modulo() = default;

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {left, right};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			size_t lidx = left->generateTAC(tac_table);
			size_t ridx = right->generateTAC(tac_table);
			
			return tac_table.add<tac::Mod>(lidx, ridx, p_owner);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("( +, {}, {})", left->__debug_string(), right->__debug_string());
		}
	
	private:

	};

} // namespace fl