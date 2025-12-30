#pragma once 

#include "blocks/block.hpp"

namespace fl
{

	class SyntaxTree : public ASTNode
	{
	private:

		Block* m_root;

	public:

		SyntaxTree(Block* root);
		~SyntaxTree();

		inline void generateTAC() const
		{
			m_root->generateTAC();
		}

		std::string __debug_string() const override 
		{
			return std::format("root: ", m_root->__debug_string());
		}
	};
	
} // namespace fl