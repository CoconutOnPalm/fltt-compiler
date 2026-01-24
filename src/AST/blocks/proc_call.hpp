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

		size_t size() const { return param_ids.size(); }

		std::vector<std::shared_ptr<ASTNode>> getChildren()
		{
			return {};
		}

		size_t generateTAC(TACTable& tac_table) const override
		{
			if (callee.empty())
			{
				panic("internal compiler error: callee not set in one of proc calls");
			}
			
			size_t param = 0;
			for (size_t i = 0; const std::string& id : param_ids)
			{
				// param = tac_table.add<tac::LD>(id, p_owner);
				tac_table.add<tac::Param>(id, callee, i++, p_owner);
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
		std::shared_ptr<Params> args;

	public:

		ProcCall(const std::string_view proc, Params&& _args)
			: procedure_id(proc), args(std::make_shared<Params>(std::move(_args)))
		{
			args->setCallee(procedure_id);
		}

		~ProcCall() = default;

		virtual std::vector<std::shared_ptr<ASTNode>> getChildren() override
		{
			std::shared_ptr<ASTNode> ast_params = args;
			return {ast_params};
		}

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			if (tac_table.getProcInfo(procedure_id) != args->size())
				panic("invalid number of arguments passed to '{}' at '{}' - required={}, provided={}", procedure_id, p_owner, args->size(), tac_table.getProcInfo(procedure_id));
			
			args->generateTAC(tac_table);			
			tac_table.add<tac::Call>(procedure_id, p_owner);
			return 0;
		}

		virtual std::string __debug_string() const override
		{
			return std::format("call {}", procedure_id);
		}

	};
	
} // namespace fl
