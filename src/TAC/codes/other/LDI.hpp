#pragma once

#include <cstdint>
#include <print>
#include <format>
#include <bit>

#include "../../tac.hpp"

#include "../../../ASM/instructions/inc.hpp"
#include "../../../ASM/instructions/shl.hpp"
#include "../../../ASM/instructions/rst.hpp"


namespace fl::tac
{
	
	class LDI : public TAC
	{

		uint64_t value {0};

	public:

		LDI(const uint64_t immediate, const std::string_view owning_proc)
			: TAC(owning_proc), value(immediate)
		{}

		virtual ~LDI() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::CONSTANT, p_owning_procedure);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			const REG reg = regalloc.allocImmediate(p_index);
			setupImmediate(reg, asm_table);
		}

		virtual std::string __debug_string() const
		{
			return std::format("LDI {}", value);
		}

	private:

		void setupImmediate(const REG reg, ASMTable& asm_table) const
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
	
	};
	
} // namespace fl
