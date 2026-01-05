#pragma once

#include <vector>
#include <memory>

#include "../ASTNode.hpp"


namespace fl::ast
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

		size_t generateTAC(TACTable& tac_table) const override
		{ 
			size_t back = 0;
			for (const auto& statement : block)
			{
				back = statement->generateTAC(tac_table);
			}

			return back;
		}
	
		std::string __debug_string() const override 
		{
			return std::format("[block] stmt count={}", block.size());
		}
	};
	
} // namespace fl