#pragma once


#include <vector>

#include "triple.hpp"
#include "../symbol/symbol_table.hpp"


namespace fl
{
	struct TACTableRef
	{
		const uint64_t index;
	};

	class TACTable
	{
	private:

		std::vector<Triple> tac;
		std::vector<TACTableRef> ids;
		uint64_t counter {};

	public:

		TACTable();
		~TACTable() = default;

		TACTableRef pushExpression(const Operator op, const tacval_t left, const tacval_t right);

		void addSymbolTable(const SymbolTable& symbol_table);

		void clear();

		void _debug_print() const;

	private:
	};

} // namespace fl
