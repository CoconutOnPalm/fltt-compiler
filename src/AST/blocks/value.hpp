#pragma once

#include <cstdint>
#include <string>
#include <format>

#include "../ASTNode.hpp"

#include "../../TAC/codes/other/LDI.hpp"
#include "../../TAC/codes/other/LD.hpp"


namespace fl::ast
{

	class Number : public ASTNode
	{
	public:

		const uint64_t val;

	public:

		Number(const uint64_t val) : val(val) {}

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{ 
			return tac_table.add<tac::LDI>(val, p_owner);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("{}", val);
		}
	};

	struct Identifier : public ASTNode
	{
	public:

		const std::string identifier;

	public:

		Identifier(const std::string& id) : identifier(id) {}

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
	
} // namespace fl
