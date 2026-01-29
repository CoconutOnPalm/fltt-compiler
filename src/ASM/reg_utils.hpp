#pragma once

#include <cln/cln.h>

#include "asm_table.hpp"
#include "memory/register.hpp"


namespace fl
{
	
	void setupImmediate(const uint64_t value, const REG reg, ASMTable& asm_table);
	void setupImmediate(const cln::cl_I value, const REG reg, ASMTable& asm_table);
	
} // namespace fl
