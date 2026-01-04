#pragma once 

#include "blocks/block.hpp"

namespace fl
{

	class SyntaxTree : public ASTNode
	{
	private:

		ast::Block* m_root;

	public:

		SyntaxTree(ast::Block* root);
		~SyntaxTree();

		inline size_t generateTAC(TACTable& tac_table) const
		{
			return m_root->generateTAC(tac_table);
		}

		std::string __debug_string() const override 
		{
			return std::format("root: ", m_root->__debug_string());
		}
	};
	
} // namespace fl