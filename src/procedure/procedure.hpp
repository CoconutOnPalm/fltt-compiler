#pragma once


#include <string>
#include <cstdint>

#include "../AST/blocks/block.hpp"
#include "../symbol/symbol_table.hpp"


namespace fl
{
	
	class Procedure
	{
	public:

		const std::string name;
		
	private:
		
		const std::shared_ptr<SymbolTable> m_symbol_table;
		const std::shared_ptr<ast::Block> m_body;
		uint64_t m_call_count {0};

		size_t m_alloc_size {0};

	public:

		Procedure(const std::string_view name, SymbolTable* symbol_table_ptr, ast::Block* body_ptr);
		Procedure(Procedure&& procedure) = default;
		~Procedure() = default;

		void makeInBlockDeclarations();

		size_t assignMemoryBlock(const size_t memshift);
		inline size_t allocSize() const { return m_alloc_size; }

		size_t generateTAC(TACTable& tac_table) const;

		uint64_t callCount() const;

		void __debug_print() const;

	private:
	};

} // namespace fl
