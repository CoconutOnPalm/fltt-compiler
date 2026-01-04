#include "syntax_tree.hpp"

namespace fl
{
    SyntaxTree::SyntaxTree(ast::Block* root)
    {
		m_root = root;
    }

    SyntaxTree::~SyntaxTree()
    {
		delete m_root;
    }

	
} // namespace fl
