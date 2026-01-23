#include "asm_table.hpp"

#include <print>


namespace fl
{

	void ASMTable::generate(std::ofstream& ofstr)
	{
		this->updateInstructionIndicies();
		this->updateJumpPositions();
		
		for (size_t i = 0; const auto& asm_ptr : m_instructions)
		{
			std::string generated = asm_ptr->generate();
			std::println("  {:2}| {}", i++, generated);

			if (!generated.empty())
				std::println(ofstr, "{}", generated);
		}
	}

    void ASMTable::updateInstructionIndicies()
    {
		for (size_t pos = 0; std::unique_ptr<Instruction>& asm_ptr : m_instructions)
		{
			ins::LABEL* cast_result = dynamic_cast<ins::LABEL*>(asm_ptr.get());
			if (cast_result == nullptr)
			{
				pos += asm_ptr->size();
				continue;
			}

			m_labels[cast_result->id] = pos;
		}
    }

    void ASMTable::updateJumpPositions()
    {
		for (std::unique_ptr<Instruction>& asm_ptr : m_instructions)
		{
			asm_ptr->updateJumpDestination(m_labels);
		}
    }
	
} // namespace fl

