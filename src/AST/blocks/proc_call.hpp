#pragma once

#include <vector>

#include "../ASTNode.hpp"
#include "../../symbol/argument.hpp"
#include "../../TAC/codes/other/param.hpp"
#include "../../TAC/codes/other/call.hpp"


namespace fl::ast
{
	class Params : public ASTNode
	{	
	private:

		std::vector<std::string> param_ids;
		std::string callee;

	public:

		Params() = default;
		~Params() = default;

		void setCallee(const std::string_view identifier) { callee = identifier; std::println("callee set"); }
		void add(const std::string& identifier) { param_ids.push_back(identifier); }

		std::vector<std::shared_ptr<ASTNode>> getChildren()
		{
			return {};
		}

		size_t generateTAC(TACTable& tac_table) const override
		{
			if (callee.empty())
			{
				std::println("[debug] callee = {}", callee);
				for (const std::string_view str : param_ids)
					std::println("param {}", str);
				panic("internal compiler error: callee not set in one of proc calls");
			}
			
			size_t param = 0;
			for (size_t i = 0; const auto& id : param_ids)
			{
				param = tac_table.add<tac::LD>(id);
				tac_table.add<tac::Param>(param, callee, i++);
			}

			return param;
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
		Params args;

	public:

		ProcCall(const std::string_view proc, Params&& _args)
			: procedure_id(proc), args(std::move(_args))
		{ 
			std::println("[debug]: {}", procedure_id);
			args.setCallee(procedure_id);
		}

		~ProcCall() = default;

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			return {};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			args.generateTAC(tac_table);			
			tac_table.add<tac::Call>(procedure_id);
			return 0;
		}

		virtual std::string __debug_string() const override
		{
			return std::format("call {}", procedure_id);
		}

	};
	
} // namespace fl
