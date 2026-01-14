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
			tac_ptr->generateASM(*asm_table, regalloc, symbol_tables);
		}
		asm_table->add<ins::HALT>();
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
