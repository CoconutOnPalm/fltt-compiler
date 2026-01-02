#pragma once

#include <vector>

#include "../ASTNode.hpp"
#include "../../symbol/argument.hpp"


namespace fl
{
	class ArgsDecl
	{	
	private:

		std::vector<Argument> args;

	public:

		ArgsDecl() = default;
		~ArgsDecl() = default;

		void add(const Argument& arg) { args.push_back(arg); }

	};
	
	class ProcDecl : ASTNode
	{
	private:

		const std::string procedure_id;
		const ArgsDecl args;

	public:

		ProcDecl(const std::string_view proc, ArgsDecl&& args)
			: procedure_id(proc), args(std::move(args))
		{}
		~ProcDecl() = default;

		void generateTAC() const 
		{
			std::println("func {}", procedure_id);
		}

		std::string __debug_string() const
		{
			return std::format("func {}", procedure_id);
		}

	};
	
} // namespace fl
