#pragma once

#include <vector>
#include <memory>
#include <map>

#include "tac.hpp"
#include "../symbol/symbol_table.hpp"

namespace fl
{

	class TACTable
	{
	private:

		std::vector<std::unique_ptr<TAC>> m_tac_table;

		// shared_ptr to extend its lifetime (used also in RegAlloc)
		std::shared_ptr<std::vector<TACInfo>> m_tac_info;

	public:

		TACTable();
		~TACTable() = default;
	
		template <typename CT, typename... Args>
		size_t add(Args&&... args)
		{
			auto& new_elem = m_tac_table.emplace_back(std::make_unique<CT>(std::forward<Args>(args)...));
			m_tac_info->push_back(new_elem->getSelfInfo());
			return m_tac_table.size() - 1;
		}

		void updateNextUse();
		void typeCheck(std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables);

		void generateASM(std::shared_ptr<ASMTable> asm_table, const size_t stack_ptr, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const;

		void __debug_print() const;
	};
	
} // namespace fl
