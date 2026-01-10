#pragma once

#include <memory>
#include <vector>

#include "instruction.hpp"


namespace fl
{
	
	class ASMTable
	{
	private:

		std::vector<std::unique_ptr<Instruction>> m_instructions;
	
	public:

		ASMTable() = default;
		~ASMTable() = default;

		template <typename InstrT, typename... Args>
		void add(Args&&... args)
		{
			m_instructions.emplace_back(std::make_unique<InstrT>(std::forward<Args>(args)...));
		}

		void generate() const;
	
	};
	
} // namespace fl
