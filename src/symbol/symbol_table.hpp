#pragma once

#include <memory>
#include <map>
#include <vector>

#include "symbol.hpp"
#include "array.hpp"
#include "variable.hpp"
#include "../utils/panic.hpp"


namespace fl
{
	
	class SymbolTable
	{
	private:

		std::map<std::string, std::unique_ptr<Symbol>> m_symbol_table;
		size_t m_size;

	public:

		SymbolTable() = default;
		~SymbolTable() = default;

		SymbolTable(const SymbolTable& ) = delete;
		SymbolTable& operator=(const SymbolTable& ) = delete;
		
		SymbolTable(SymbolTable&& other) : m_symbol_table(std::move(other.m_symbol_table)) {}

		template <class SymT, typename... Args>
		inline void add(const std::string& name, Args&&... args)
		{
			// commented out - allow redeclarations (for now)
			// if (m_symbol_table.contains(name))
			// 	panic("redeclaration of the variable '{}'", name);
				
			m_symbol_table.emplace(name, std::make_unique<SymT>(name, args...));
			m_size += m_symbol_table[name]->memsize;
		}

		template <class SymT>
		inline void add(const Symbol* symbol)
		{	
			m_symbol_table.emplace(symbol->name, symbol->clone());
		}

		const Symbol& get(const std::string& name) const;
		inline const Symbol& operator[](const std::string& name) const { return get(name); }
		
		void assignMemory(const size_t shift);

		inline size_t size() const { return m_size; }

		
		void __debug_print() const;
	};

} // namespace fl

