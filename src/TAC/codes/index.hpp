#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../tac.hpp"


namespace fl::tac
{
	
	class Index : public TAC
	{

		std::string lvalue_id;	// id in symbol table
		size_t rvalue_index;	// index in TAC table

	public:

		Index(const std::string_view lvalue, const size_t rvalue)
			: lvalue_id(lvalue), rvalue_index(rvalue)
		{}

		virtual ~Index() = default;

		virtual void generateASM() const override
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("{}[{}]", lvalue_id, rvalue_index);
		}
	};
	
} // namespace fl
