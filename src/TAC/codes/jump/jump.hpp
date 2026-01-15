#pragma once


#include <memory>

#include "../../tac.hpp"
#include "../other/label.hpp"

#include "../../../ASM/instructions/jump.hpp"


namespace fl::tac
{

	class JUMP : public TAC
	{
	private:

		std::shared_ptr<size_t> label_id;
		
	public:

		JUMP(std::shared_ptr<size_t> label, const std::string_view owning_proc)
			: TAC(owning_proc), label_id(label)
		{}
		virtual ~JUMP() = default;

		
		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::JUMP, p_owning_procedure);
		}
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			asm_table.add<ins::JUMP>(*label_id);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("JUMP --> {}", *(label_id.get()));
		}
	};
	
} // namespace fl::tac
