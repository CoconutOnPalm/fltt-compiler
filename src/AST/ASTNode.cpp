#include "ASTNode.hpp"

namespace fl
{
	
	void ASTNode::declareInBlock(SymbolTable& symbol_table)
	{
		auto children = getChildren();
		for (size_t i = 0; const auto& child_ptr : children)
		{
			child_ptr->declareInBlock(symbol_table);
			i++;
		}
	}

}