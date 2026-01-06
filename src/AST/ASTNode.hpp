#pragma once

#include <print> // propagate include down

#include "../TAC/tac_table.hpp"
#include "../symbol/symbol_table.hpp"

namespace fl
{
	
	class ASTNode
	{
	public:

		ASTNode() = default;
		virtual ~ASTNode() = default;

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() = 0;;
		virtual size_t generateTAC(TACTable& tac_table) const = 0;

		virtual void declareInBlock(SymbolTable& symbol_table);

		virtual std::string __debug_string() const = 0;
	};
	
} // namespace fl