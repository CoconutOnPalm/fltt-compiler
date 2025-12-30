#pragma once

#include <format>
#include <map>
#include <variant>

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
		const static std::map<fl::Operator, std::string> mapper = {
			{fl::Operator::ASSIGN, 	":="},
			{fl::Operator::INDEX, 	"[]"},
			{fl::Operator::ADD, 	"+" },
			{fl::Operator::SUB, 	"-" },
			{fl::Operator::MULT, 	"*" },
			{fl::Operator::DIV, 	"/" },
			{fl::Operator::MOD, 	"%" },
		};

		return std::formatter<std::string>::format(std::format("{}", mapper.at(op)), ctx);
	}
};

template <typename char_T>
struct std::formatter<fl::CondOp, char_T> : std::formatter<std::string>
{
	auto format(const fl::CondOp& cond, std::format_context& ctx) const
	{
		const static std::map<fl::CondOp, std::string> mapper = {
			{fl::CondOp::EQ, 	"=="},
			{fl::CondOp::NEQ, 	"!="},
			{fl::CondOp::GT, 	">" },
			{fl::CondOp::LT, 	"<" },
			{fl::CondOp::GEQ, 	">="},
			{fl::CondOp::LEQ, 	"<="},
		};

		return std::formatter<std::string>::format(std::format("{}", mapper.at(cond)), ctx);
	}
};