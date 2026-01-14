#pragma once

#include "asm_table.hpp"
#include "memory/register.hpp"


namespace fl
{
	
	void setupImmediate(const uint64_t value, const REG reg, ASMTable& asm_table);
	
} // namespace fl
