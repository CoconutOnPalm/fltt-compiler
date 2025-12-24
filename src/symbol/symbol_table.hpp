#pragma once

#include <memory>
#include <map>

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

		template <class SymT, typename... Args>
		void add(const std::string& name, Args&&... args)
		{
			if (symbol_table.contains(name))
				panic("redeclaration of the variable '{}'", name);

			symbol_table[name] = std::make_unique<SymT>(name, std::forward<Args>(args)...);
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

