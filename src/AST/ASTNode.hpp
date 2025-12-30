#pragma once

#include <print> // propagate include down

namespace fl
{
	
	class ASTNode
	{
	public:

		ASTNode() = default;
		virtual ~ASTNode() = default;

		virtual void generateTAC() const = 0;

		virtual std::string __debug_string() const = 0;
	};
	
} // namespace fl