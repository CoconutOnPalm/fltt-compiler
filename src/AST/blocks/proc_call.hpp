#pragma once

#include <vector>

#include "../ASTNode.hpp"
#include "../../symbol/argument.hpp"


namespace fl::ast
{
	class Params : public ASTNode
	{	
	private:

		std::vector<std::string> param_ids;

	public:

		Params() = default;
		~Params() = default;

		void add(const std::string& identifier) { param_ids.push_back(identifier); }

		std::vector<std::shared_ptr<ASTNode>> getChildren()
		{
			return {};
		}

		size_t generateTAC(TACTable& tac_table) const override
		{
			for (const auto& id : param_ids)
			{
				std::println("param {}", id);
			}
			return 0;
		}

		std::string __debug_string() const 
		{
			return std::format("param count = {}", param_ids.size());
		}
	};
	
	class ProcCall : public ASTNode
	{
	private:

		const std::string procedure_id;
		const Params args;

	public:

		ProcCall(const std::string_view proc, Params&& args)
			: procedure_id(proc), args(std::move(args))
		{}
		~ProcCall() = default;

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			args.generateTAC(tac_table);
			std::println("call {}", procedure_id);
			return 0;
		}

		virtual std::string __debug_string() const override
		{
			return std::format("call {}", procedure_id);
		}

	};
	
} // namespace fl
