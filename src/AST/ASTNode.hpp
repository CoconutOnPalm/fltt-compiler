#pragma once

#include <print> // propagate include down

#include "../TAC/tac_table.hpp"

namespace fl
{
	
	class ASTNode
	{
	public:

		ASTNode() = default;
		virtual ~ASTNode() = default;

		virtual size_t generateTAC(TACTable& tac_table) const = 0;

		virtual std::string __debug_string() const = 0;
	};
	
} // namespace fl