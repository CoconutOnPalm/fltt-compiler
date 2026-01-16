#include "procedure.hpp"

#include "../TAC/codes/other/proc_enter.hpp"
#include "../TAC/codes/other/proc_exit.hpp"
#include "../TAC/codes/other/halt.hpp"


namespace fl
{

	Procedure::Procedure(const std::string_view name, std::shared_ptr<SymbolTable> symbol_table_ptr, ast::Block* body_ptr)
		: name(name), m_symbol_table(symbol_table_ptr), m_body(body_ptr), m_call_count(0)
	{
		m_alloc_size = m_symbol_table->size();
	}

	void Procedure::makeInBlockDeclarations()
	{
		m_body->declareInBlock(*m_symbol_table);
	}

	size_t Procedure::assignMemoryBlock(const size_t memshift)
	{
		m_symbol_table->assignMemory(memshift);
		return m_symbol_table->size();
	}

	size_t Procedure::generateTAC(TACTable& tac_table) const
	{
		if (name != config::prog_entry_name)
			tac_table.add<tac::ProcEnter>(name);

		m_body->generateTAC(tac_table);

		if (name != config::prog_entry_name)
			return tac_table.add<tac::ProcExit>(name);
		else
			return tac_table.add<tac::Halt>();
	}

	uint64_t Procedure::callCount() const
	{
		return m_call_count;
	}

	void Procedure::__debug_print() const
	{
		std::println("func {}(...)", name);
		m_symbol_table->__debug_print();
	}

} // namespace fl
