#include "reg_utils.hpp"

#include "instructions/rst.hpp"
#include "instructions/inc.hpp"
#include "instructions/shl.hpp"

namespace fl
{

	void setupImmediate(const uint64_t value, const REG reg, ASMTable& asm_table)
	{
		asm_table.add<ins::RST>(reg);
		const size_t width = std::bit_width(value);
		for (size_t i = 0; i < width; i++)
		{
			bool bit = (value >> (width - i - 1)) & 1u;

			if (bit)
				asm_table.add<ins::INC>(reg);

			if (i < width - 1) // don't shift the last bit
				asm_table.add<ins::SHL>(reg);
		}
	}

} // namespace fl
