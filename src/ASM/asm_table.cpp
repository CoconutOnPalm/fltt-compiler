#include "asm_table.hpp"

#include <print>
#include <ranges>

#include "instructions/noop.hpp"


namespace fl
{

    std::pair<uint64_t, uint64_t> ASMTable::countIO() const
    {
		uint64_t reads = 0;
		uint64_t writes = 0;

		for (const std::unique_ptr<Instruction>& instr : m_instructions)
		{
			reads += instr->generate().starts_with("READ");
			writes += instr->generate().starts_with("WRITE");
		}

		return {reads, writes};
    }


	void ASMTable::generate(std::ofstream& ofstr)
	{
		this->removeRedundantInstructions();
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

    void ASMTable::generate(std::vector<std::string>& instructions)
    {
		this->removeRedundantInstructions();
		this->updateInstructionIndicies();
		this->updateJumpPositions();
		
		for (size_t i = 0; const auto& asm_ptr : m_instructions)
		{
			std::string generated = asm_ptr->generate();

			if (!generated.empty())
				instructions.push_back(generated);
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

    void ASMTable::removeRedundantInstructions()
    {
		// remove double swaps
		for (size_t i = 0; i < m_instructions.size() - 1; i++)
		{
			if (m_instructions[i]->generate().starts_with("SWP") && m_instructions[i]->generate() == m_instructions[i + 1]->generate())
			{
				// double stack of 
				// SWP <reg>
				// SWP <reg>
				// instructions - remove as they cancel each other
				m_instructions[i] = 	std::make_unique<ins::NOOP>();
				m_instructions[i + 1] = std::make_unique<ins::NOOP>();
				i++;
			}
		}
    }
	
} // namespace fl

