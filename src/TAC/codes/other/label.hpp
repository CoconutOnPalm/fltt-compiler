#pragma once

#include <cstdint>
#include <print>
#include <format>
#include <memory>

#include "../../tac.hpp"

#include "../../../ASM/instructions/label.hpp"

namespace fl::tac
{
	
	class Label : public TAC
	{
	private:

		inline static uint64_t uuid_selector { 0 };

		const std::string name;
		std::shared_ptr<uint64_t> uuid;

	public:

		Label(const std::string_view _name, std::shared_ptr<uint64_t> uuid, const std::string_view owning_proc)
			: TAC(owning_proc), name(_name), uuid(uuid)
		{ *uuid = nextUUID(); }
		~Label() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::LABEL, p_owning_procedure);
		}

		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			asm_table.add<ins::LABEL>(*uuid);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("# {} [{}]", name, *uuid);
		}


		static uint64_t nextUUID()
		{
			return uuid_selector++;
		}

	};
	
} // namespace fl
