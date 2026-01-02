#pragma once

#include <vector>
#include <string>
#include <memory>
#include <print>

#include "../utils/const.hpp"

namespace fl
{

	class PTNode
	{
	private:

		std::string procedure_id;
		std::vector<std::shared_ptr<PTNode>> subprocedures;
		PTNode* caller {nullptr};

	public:

		PTNode(const std::string& identifier)
			: procedure_id(identifier)
		{}

		void addCallee(PTNode* callee)
		{
			subprocedures.emplace_back(callee);
		}

		void assignCaller(PTNode* caller)
		{
			this->caller = caller;
		}

		void __debug_print() const 
		{
			std::println("{}:", procedure_id);
			for (const auto& subproc : subprocedures)
			{
				std::print("  ");
				subproc->__debug_print();
			}
		}
		
	};
	

	
	class ProcedureTree
	{
	private:

		PTNode main;

	public:

		// ProcedureTree();
		// ~ProcedureTree();

	};
	
} // namespace fl
