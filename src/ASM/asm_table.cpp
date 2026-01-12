#include "asm_table.hpp"

#include <print>


namespace fl
{

	void ASMTable::generate(std::ofstream& ofstr) const
	{
		for (size_t i = 0; const auto& asm_ptr : m_instructions)
		{
			std::string generated = asm_ptr->generate();
			std::println("  {:2}| {}", i++, generated);

			ofstr << std::format("{}\n", generated);
		}
	}
	
} // namespace fl

