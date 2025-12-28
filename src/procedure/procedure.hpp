#pragma once


#include <string>
#include <cstdint>


#include "../symbol/symbol_table.hpp"


namespace fl
{
	
	class Procedure
	{
	public:

		const std::string name;
		const SymbolTable symbol_table;

	private:

		uint64_t m_call_count {0};

	public:

		Procedure(const std::string_view name, SymbolTable&& symb_table);
		Procedure(Procedure&& procedure) = default;
		~Procedure() = default;

		uint64_t callCount() const;

	private:
	};

} // namespace fl
