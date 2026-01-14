#pragma once

#include <cstdint>
#include <string>
#include <format>

#include "../ASTNode.hpp"

#include "../../TAC/codes/other/LDI.hpp"
#include "../../TAC/codes/other/LD.hpp"


namespace fl::ast
{

	class Identifier : public ASTNode
	{
	public:

		const std::string identifier;

	public:

		Identifier(const std::string_view id) : identifier(id) {}
		virtual ~Identifier() = default;

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			return tac_table.add<tac::LD>(identifier, p_owner);
		}

		virtual std::string __debug_string() const override
		{
			return identifier;
		}
	};

} // namespace fl::ast