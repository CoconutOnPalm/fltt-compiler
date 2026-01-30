#include "tac_table.hpp"

#include <print>

#include "../ASM/instructions/halt.hpp"


namespace fl
{
    TACTable::TACTable()
		: m_tac_info(std::make_shared<std::vector<TACInfo>>())
    {}

    void TACTable::updateNextUse()
    {
		for (const auto& tac_ptr : m_tac_table)
		{
			tac_ptr->updateNextUse(*m_tac_info);
		}
    }

    void TACTable::typeCheck(std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables)
    {
		for (const auto& tac_ptr : m_tac_table)
		{
			tac_ptr->typeCheck(*m_tac_info, symbol_tables);
		}
    }

    void TACTable::generateASM(std::shared_ptr<ASMTable> asm_table, const size_t stack_ptr, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const
    {
		RegAlloc regalloc(m_tac_info, asm_table, stack_ptr);
		for (size_t i = 0; const auto& tac_ptr : m_tac_table)
		{
			tac_ptr->generateASM(*asm_table, regalloc, symbol_tables, *m_tac_info);
			// regalloc.__debug_print();
			regalloc.flushTemporaryTAC();
			
			if (m_tac_info->at(tac_ptr->p_index).block_terminator)
			{
				regalloc.flushRegisters();
			}
		}
		asm_table->add<ins::HALT>();
    }

    void TACTable::declProc(const std::string_view proc_id, const size_t argc)
    {
		if (m_argcount_map.contains(std::string(proc_id)))
			panic("redeclaration of proc '{}'", proc_id);

		m_argcount_map.emplace(proc_id, argc);
    }

    size_t TACTable::getProcInfo(const std::string& proc_id)
    {
		if (!m_argcount_map.contains(proc_id))
			panic("undefined procedure '{}'", proc_id);

		return m_argcount_map[proc_id];
    }


    void TACTable::__debug_print() const
    {
		std::println("THREE ADRESS CODE");
		for (size_t i = 0; const auto& tac_ptr : m_tac_table)
		{
			std::println("  {:2}| {}", i++, tac_ptr->__debug_string());
		}

		std::println();
		std::println("NEXTUSE INFO");
		for (size_t i = 0; TACInfo tac_info : *m_tac_info)
		{
			std::print("  {:2}| ", i++);
			auto usages = tac_info.usages();
			for (size_t usage : usages)
				std::print("{}, ", usage);
			// for (size_t j = 0; j < m_tac_info.size(); j++)
			// 	if (tac_info.hasNextUse(j))
			// 		std::print("{}, ", tac_info.nextUsage(j));
			std::println();
		}
    }
	
} // namespace fl
