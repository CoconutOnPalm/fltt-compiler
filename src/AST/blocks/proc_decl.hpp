#pragma once

#include <vector>

#include "../ASTNode.hpp"
#include "../../symbol/argument.hpp"
#include "../../TAC/codes/other/label.hpp"


namespace fl::ast
{
	class ArgsDecl
	{	
	private:

		std::vector<Argument> args;

	public:

		ArgsDecl() = default;
		~ArgsDecl() = default;

		void add(const Argument& arg) { args.push_back(arg); }
		const std::vector<Argument>& get() const { return args; }

		size_t size() const { return args.size(); }

		std::string __debug_string() const 
		{
			std::string out;
			for (const Argument& arg : args)
				out += std::format("{}, ", arg.__debug_string());
			return out;
		}
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

		inline std::string name() const { return procedure_id; }
		inline const std::vector<Argument>& getArgs() const { return args.get(); }

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			return 0;
		}

		virtual std::string __debug_string() const override
		{
			return std::format("func @{}", procedure_id);
		}

	};
	
} // namespace fl
