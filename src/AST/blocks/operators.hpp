#pragma once

#include <format>
#include <map>
#include <variant>

#include "../../utils/panic.hpp"


namespace fl
{
	// <id, num>
	using val_t = std::variant<std::string_view, uint64_t>; 

	enum class Operator
	{
		ASSIGN,	// x = y
		INDEX,	// x[i]
		ADD,	// x + y
		SUB,	// x - y
		MULT,	// x * y
		DIV,	// x / y
		MOD,	// x mod y
	};

	enum class CondOp
	{
		EQ,		// ==
		NEQ,	// !=
		GT,		// >
		LT,		// <
		GEQ,	// >=
		LEQ,	// <=
	};
	
} // namespace fl


template <typename char_T>
struct std::formatter<fl::Operator, char_T> : std::formatter<std::string>
{
	auto format(const fl::Operator& op, std::format_context& ctx) const
	{
		switch (op)
        {
        case fl::Operator::ASSIGN:
            return std::formatter<std::string>::format(":=", ctx);
        case fl::Operator::INDEX:
            return std::formatter<std::string>::format("[]", ctx);
        case fl::Operator::ADD:
            return std::formatter<std::string>::format("+", ctx);
        case fl::Operator::SUB:
            return std::formatter<std::string>::format("-", ctx);
        case fl::Operator::MULT:
            return std::formatter<std::string>::format("*", ctx);
        case fl::Operator::DIV:
            return std::formatter<std::string>::format("/", ctx);
        case fl::Operator::MOD:
            return std::formatter<std::string>::format("%", ctx);
        default:
            fl::panic("internal compiler error: Operator not defined");
            return std::formatter<std::string>::format("?", ctx);
        }
	}
};

template <typename char_T>
struct std::formatter<fl::CondOp, char_T> : std::formatter<std::string>
{
	auto format(const fl::CondOp& cond, std::format_context& ctx) const
	{
		switch (cond)
		{
		case fl::CondOp::EQ:
			return std::formatter<std::string>::format("==", ctx);
		case fl::CondOp::NEQ:
			return std::formatter<std::string>::format("!=", ctx);
		case fl::CondOp::GT:
			return std::formatter<std::string>::format(">", ctx);
		case fl::CondOp::LT:
			return std::formatter<std::string>::format("<", ctx);
		case fl::CondOp::GEQ:
			return std::formatter<std::string>::format(">=", ctx);
		case fl::CondOp::LEQ:
			return std::formatter<std::string>::format("<=", ctx);
		default:
			fl::panic("internal compiler error: Conditional operator not defined");
			return std::formatter<std::string>::format("?", ctx);
		}
	}
};