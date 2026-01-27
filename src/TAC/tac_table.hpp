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

		std::map<std::string, size_t> m_argcount_map;

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

		void declProc(const std::string_view proc_id, const size_t argc);
		size_t getProcInfo(const std::string& proc_id); // currently returns only argcount

		const std::vector<TACInfo>&getTACInfoTable() const { return *m_tac_info; }

		void __debug_print() const;
	};
	
} // namespace fl
