#pragma once


#include <string>

#include "tac_info.hpp"
#include "../symbol/symbol_table.hpp"

#include "../ASM/memory/register_allocator.hpp"
#include "../ASM/asm_table.hpp"



namespace fl
{
	
	class TAC
	{
	private:

		inline static size_t s_global_index {0};
	
	protected:

		const size_t p_index;
		const std::string p_owning_procedure;

	public:

		TAC(const std::string_view owning_proc) 
			: p_index(s_global_index++), p_owning_procedure(owning_proc) 
		{}
		virtual ~TAC() = default;
		
		virtual TACInfo getSelfInfo() const = 0;
		virtual void updateNextUse(std::vector<TACInfo>& info_table) const { /* empty as default */ };
		virtual void typeCheck(const std::vector<TACInfo>& info_table, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const 
		{ /* empty as default */ };
		
		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables, std::vector<TACInfo>& info_table) const = 0;

		virtual std::string __debug_string() const = 0;

	private:

		friend class TACTable;
		
	};
	
} // namespace fl
