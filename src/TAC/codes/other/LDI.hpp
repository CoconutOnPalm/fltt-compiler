#pragma once

#include <cstdint>
#include <print>
#include <format>
#include <bit>

#include "../../tac.hpp"

#include "../../../ASM/instructions/inc.hpp"
#include "../../../ASM/instructions/shl.hpp"
#include "../../../ASM/instructions/rst.hpp"

#include "../../../ASM/reg_utils.hpp"


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
			return TACInfo(TACType::CONSTANT, p_owning_procedure, value);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, const std::vector<TACInfo>& info_table) const override
		{
			regalloc.loadImmediate(p_index, value);
		}

		virtual std::string __debug_string() const
		{
			return std::format("LDI {}", value);
		}
	
	};
	
} // namespace fl
