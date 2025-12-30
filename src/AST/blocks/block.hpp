#pragma once

#include <vector>
#include <memory>

#include "../ASTNode.hpp"


namespace fl
{

	class Block : public ASTNode
	{
	protected:

		std::vector<std::shared_ptr<ASTNode>> block;
	
	public:

		Block() = default;
		virtual ~Block() = default;

		void addStatement(ASTNode* stmt_owning_ptr)
		{
			block.emplace_back(std::shared_ptr<ASTNode>(stmt_owning_ptr));
		}

		void generateTAC() const
		{ 
			std::println("[block]:");
			for (const auto& statement : block)
			{
				std::print("  ");
				statement->generateTAC();
			}
		}
	
		std::string __debug_string() const override 
		{
			return std::format("[block] stmt count={}", block.size());
		}
	};
	
} // namespace fl