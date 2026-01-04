#pragma once

#include <cstdint>
#include <string>
#include <format>

#include "../ASTNode.hpp"

#include "../../TAC/codes/LDI.hpp"
#include "../../TAC/codes/LD.hpp"


namespace fl::ast
{

	class Number : public ASTNode
	{
	private:

		const uint64_t val;

	public:

		Number(const uint64_t val) : val(val) {}

		size_t generateTAC(TACTable& tac_table) const 
		{ 
			return tac_table.add<tac::LDI>(val);
		}

		std::string __debug_string() const override
		{
			return std::format("{}", val);
		}
	};

	struct Identifier : public ASTNode
	{
	private:

		const std::string identifier;

	public:

		Identifier(const std::string& id) : identifier(id) {}

		size_t generateTAC(TACTable& tac_table) const override
		{ 
			return tac_table.add<tac::LD>(identifier);
		}

		std::string __debug_string() const override
		{
			return identifier;
		}
	};
	
} // namespace fl
