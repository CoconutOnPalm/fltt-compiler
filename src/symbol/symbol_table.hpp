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

		std::map<std::string, std::unique_ptr<Symbol>> symbol_table;

	public:

		SymbolTable() = default;
		~SymbolTable() = default;

		SymbolTable(const SymbolTable& other) = delete;
		SymbolTable(SymbolTable&& other) : symbol_table(std::move(other.symbol_table)) {}

		template <class SymT, typename... Args>
		inline void add(const std::string& name, Args&&... args)
		{
			symbol_table.emplace(name, std::make_unique<SymT>(name, args...));
		}

		const Symbol& get(const std::string& name) const
		{
			if (!symbol_table.contains(name))
			{
				panic("undefined variable '{}'", name);
			}

			return *(symbol_table.at(name).get());
		}

		const Symbol& operator[](const std::string& name) const
		{
			return get(name);
		}

		std::vector<std::string> getDeclaredVariables() const 
		{
			std::vector<std::string> names; names.reserve(symbol_table.size());
			for (const auto& [name, _] : symbol_table)
				names.push_back(name);
			return names;
		}

		inline void clear()
		{
			symbol_table.clear();
		}
		
		void _debug_print() const
		{
			for (const auto& [name, symbol] : symbol_table)
			{
				std::println("[{:20}]: {}", name, symbol->_debug_string());
			}
		}
	};

} // namespace fl

