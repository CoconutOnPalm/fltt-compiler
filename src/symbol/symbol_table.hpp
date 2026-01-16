#pragma once

#include <memory>
#include <map>
#include <vector>

#include "symbol.hpp"
#include "array.hpp"
#include "variable.hpp"
#include "argument.hpp"
#include "../utils/panic.hpp"


namespace fl
{
	
	class SymbolTable
	{
	private:

		std::map<std::string, std::unique_ptr<Symbol>> m_symbol_table;
		std::vector<std::string> m_arg_ids;
		size_t m_size {};

		uint64_t proc_uuid {};

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

		inline void addArgument(const Argument* argument)
		{
			m_symbol_table.emplace(argument->name, argument->clone());
			m_arg_ids.push_back(argument->name);
		}

		Symbol& get(const std::string& name);
		inline Symbol& operator[](const std::string& name) { return get(name); }

		std::string argAt(const size_t index);

		// inline std::map<std::string, std::unique_ptr<Symbol>>& elements() { return m_symbol_table; }
		
		inline void setUUID(const uint64_t uuid) { proc_uuid = uuid; }
		inline uint64_t UUID() const { return proc_uuid; }
		
		void assignMemory(const size_t shift);

		inline size_t size() const { return m_size; }

		
		void __debug_print() const;
	};

} // namespace fl

