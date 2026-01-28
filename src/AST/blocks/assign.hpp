#pragma once

#include "identifier.hpp"
#include "number.hpp"
#include "../../TAC/codes/expressions/assign.hpp"


namespace fl::ast
{
	
	class Assign : public ASTNode
	{
	private:

		std::shared_ptr<Identifier> lvalue;
		std::shared_ptr<ASTNode> rvalue;
	
	public:

		Assign(Identifier* lval, ASTNode* rval)
			: lvalue(lval), rvalue(rval)
		{}

		virtual ~Assign() = default;

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {lvalue, rvalue};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			size_t ridx = rvalue->generateTAC(tac_table);
			size_t lidx = lvalue->generateTAC(tac_table);

			return tac_table.add<tac::Assign>(lidx, ridx, p_owner);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("(:=, {}, {})", lvalue->__debug_string(), rvalue->__debug_string());
		}
		
	};
	
} // namespace fl::ast
