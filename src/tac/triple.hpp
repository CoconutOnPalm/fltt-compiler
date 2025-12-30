#pragma once

#include <cstdint>
#include <format>
#include <map>
#include <variant>

#include "../utils/panic.hpp"

namespace fl
{

	enum class Operator
	{
		ASSIGN,
		INDEX,
		ADD,
		SUB,
		MULT,
		DIV,
		MOD,
	};

	// <pidentifier, num, TAC index>
	using tacval_t = std::variant<std::string, uint64_t, size_t>;
	
	struct Triple
	{
		const Operator op;
		const tacval_t left;
		const tacval_t right;
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
struct std::formatter<fl::Triple, char_T> : std::formatter<std::string>
{
	void assign_variant_string(std::string& str, const fl::tacval_t variant) const
	{
		if 		(variant.index() == 0) str = std::get<0>(variant);
		else if (variant.index() == 1) str = std::format("{}", std::get<1>(variant));
		else if (variant.index() == 2) str = std::format("[{}]", std::get<2>(variant));
		else fl::panic("internal compiler error: std::formatter<Triple> - variant doesn't hold any alternative");
	}

	auto format(const fl::Triple& triple, std::format_context& ctx) const
	{
		std::string left {}, right {};
		assign_variant_string(left, triple.left);
		assign_variant_string(right, triple.right);

		return std::formatter<std::string>::format(std::format("({:2}, {}, {})", triple.op, left, right), ctx);
	}
};