#include "register_allocator.hpp"

#include <random>

#include "../../utils/panic.hpp"
#include "../../utils/logging.hpp"

#include "../instructions/load.hpp"
#include "../instructions/rload.hpp"
#include "../instructions/store.hpp"
#include "../instructions/rstore.hpp"
#include "../instructions/swp.hpp"
#include "../instructions/move.hpp"

#include "../reg_utils.hpp"


namespace fl
{
	RegAlloc::RegAlloc(std::shared_ptr<std::vector<TACInfo>> tac_info, std::shared_ptr<ASMTable> asm_table, const size_t stack_ptr)
		: m_tac_info(tac_info), m_asm_table(asm_table), m_stack_ptr(stack_ptr)
	{
	}

	// void RegAlloc::allocate(const size_t tac, const std::string_view variable_name)
	// {
	// 	// search for this tac 
	// 	for (size_t i = 0; i < m_registers.size(); i++)
	// 	{
	// 		if (m_registers[i].tac.contains(tac) && m_registers[i].assoc_variable == variable_name)
	// 		{
	// 			return;
	// 			// return static_cast<REG>(i);
	// 		}
	// 	}

	// 	// search for the first empty register
	// 	for (size_t i = 0; i < m_registers.size(); i++)
	// 	{
	// 		Register& reg = m_registers[i];
	// 		if (reg.tac.empty() && reg.assoc_variable.empty())
	// 		{
	// 			reg.tac.emplace(tac);
	// 			reg.assoc_variable = variable_name;
	// 			return;
	// 			// return static_cast<REG>(i);
	// 		}
	// 	}


	// 	std::vector<TACInfo>& tac_info = *m_tac_info; // alias

	// 	// search for the first dead tac
	// 	for (size_t i = 0; i < m_registers.size(); i++)
	// 	{
	// 		Register& reg = m_registers[i];
	// 		// if (reg.tac_index >= tac_info.size())
	// 		// 	panic("internal compier error: requested a register for TAC beyond scope");

	// 		bool all_free = reg.assoc_variable.empty();
	// 		for (size_t code : reg.tac)
	// 		{
	// 			if (tac_info[code].hasNextUse(tac))
	// 			{
	// 				all_free = false;
	// 				break;
	// 			}
	// 		}

	// 		if (all_free)
	// 		{
	// 			reg.tac.clear();
	// 			reg.assoc_variable.clear(); // redundant
	// 			reg.tac.emplace(tac);
	// 			return;
	// 		}
	// 	}

	// 	panic("out of registers");
	// }

	// void RegAlloc::assign(const REG reg, const size_t tac)
	// {
	// 	auto& rvalue = m_registers[static_cast<size_t>(get(tac))];
	// 	rvalue.assoc_variable.clear();
	// 	rvalue.tac.erase(tac);
	// 	m_registers[static_cast<size_t>(reg)].tac.emplace(tac);
	// }

	void RegAlloc::overrideRA(const size_t tac)
	{
		m_registers[0].data_type = DataType::TEMPORARY;
		m_registers[0].address = 0;
		m_registers[0].tac = tac;
	}

	REG RegAlloc::swap(const REG reg)
	{
		if (reg == REG::RA)
			return REG::RA;

		std::swap(m_registers[0], m_registers[static_cast<size_t>(reg)]);
		m_asm_table->add<ins::SWP>(reg);

		return REG::RA;
	}

	REG RegAlloc::allocPointer(const size_t tac)
	{
		REG reg = this->getEmptyRegister(tac);
		m_registers[static_cast<size_t>(reg)].data_type = DataType::POINTER;
		m_registers[static_cast<size_t>(reg)].tac = tac;
		m_registers[static_cast<size_t>(reg)].address = 0;
		return reg;
	}

	REG RegAlloc::allocImmediate(const size_t tac)
	{
		REG reg = this->getEmptyRegister(tac);
		m_registers[static_cast<size_t>(reg)].data_type = DataType::IMMEDIATE;
		m_registers[static_cast<size_t>(reg)].tac = tac;
		m_registers[static_cast<size_t>(reg)].address = 0;
		return reg;
	}

	REG RegAlloc::allocTemporary(const size_t tac)
	{
		REG reg = this->getEmptyRegister(tac);
		m_registers[static_cast<size_t>(reg)].data_type = DataType::TEMPORARY;
		m_registers[static_cast<size_t>(reg)].tac = tac;
		m_registers[static_cast<size_t>(reg)].address = 0;
		return reg;
	}

	REG RegAlloc::loadVariable(const size_t tac, const size_t addr)
	{
		// TODO: swap back just like in loadPointer
		if (addr == 0)
			panic("null ptr dereference");

		REG reg = this->getEmptyRegister(tac, addr);
		const REG reg_position = reg;

		this->swap(reg);

		m_asm_table->add<ins::LOAD>(addr);

		m_registers[0].data_type = DataType::VARIABLE;
		m_registers[0].tac = tac;
		m_registers[0].address = addr;

		this->swap(reg_position);

		return reg_position;
	}

    REG RegAlloc::storeVariable(REG reg, const size_t addr)
    {
		reg = this->swap(reg);
		m_asm_table->add<ins::STORE>(addr);
		return reg;
    }

	// warning - this destroys whatever's inside 'reg'
	REG RegAlloc::loadPointer(const size_t tac, REG reg)
	{
		REG val_reg = this->getEmptyRegister(tac, 0);
		const REG reg_position = val_reg;

		this->resetRegister(reg);

		if (reg == REG::RA) // prevent register loss
			reg = val_reg;

		this->swap(val_reg); // RA = ptr_reg

		m_asm_table->add<ins::RLOAD>(reg);


		m_registers[0].data_type = DataType::POINTER;
		m_registers[0].tac = tac;
		m_registers[0].address = 0;

		this->swap(reg_position);

		return reg_position;
	}

	REG RegAlloc::loadImmediate(const size_t tac, const uint64_t imm)
	{
		REG imm_reg = this->getEmptyRegister(tac, 0);

		m_registers[static_cast<size_t>(imm_reg)].data_type = DataType::IMMEDIATE;
		m_registers[static_cast<size_t>(imm_reg)].tac = tac;
		m_registers[static_cast<size_t>(imm_reg)].address = 0;

		setupImmediate(imm, imm_reg, *m_asm_table);
		return imm_reg;
	}

	// REG RegAlloc::load(const size_t tac, const size_t addr)
	// {
	// 	if (addr == 0)
	// 		panic("null ptr assignment");

	// 	REG reg = this->getEmptyRegister(tac, addr);
	// 	this->swap(reg);

	// 	m_asm_table->add<ins::LOAD>(addr);

	// 	m_registers[0].data_type = DataType::VARIABLE;
	// 	m_registers[0].tac = tac;
	// 	m_registers[0].address = addr;
	// 	return reg;
	// }

	// REG RegAlloc::rload(const size_t tac, const REG reg)
	// {
	// 	REG elem_reg = this->getEmptyRegister(tac, 0);
	// 	this->swap(elem_reg);

	// 	m_asm_table->add<ins::RLOAD>(reg);

	// 	m_registers[0].data_type = DataType::POINTER;
	// 	m_registers[0].tac = tac;
	// 	m_registers[0].address = 0;
	// 	return reg;
	// }

	REG RegAlloc::get(const size_t tac)
	{
		// search registers
		for (size_t i = 0; i < m_registers.size(); i++)
		{
			if (m_registers[i].tac == tac)
			{
				return static_cast<REG>(i);
			}
		}

		warning("register not found - searching the stack");

		// search the stack
		for (size_t i = 0; i < m_stack.size(); i++)
		{
			if (m_stack[i].tac == tac)
			{
				REG empty_reg = getEmptyRegister(tac);
				swap(empty_reg);
				m_asm_table->add<ins::LOAD>(m_stack_ptr + i);
			}
		}

		__debug_print();
		panic("register lost for TAC ({})", tac);
		return REG::RA;
	}

	REG RegAlloc::getValue(const size_t tac)
	{
		// search registers
		for (size_t i = 0; i < m_registers.size(); i++)
		{
			if (m_registers[i].tac == tac)
			{
				if (m_registers[i].data_type == DataType::POINTER)
				{
					return this->loadPointer(tac, static_cast<REG>(i));
				}

				return static_cast<REG>(i);
			}
		}

		warning("register not found - searching the stack");
		// stack search not implemented
		__debug_print();
		panic("register lost for TAC ({})", tac);
		return REG::RA;
	}

	void RegAlloc::resetRegister(const REG reg)
	{
		m_registers[static_cast<size_t>(reg)].data_type = DataType::NONE;
		m_registers[static_cast<size_t>(reg)].tac = empty_tac;
		m_registers[static_cast<size_t>(reg)].address = 0;
	}

	void RegAlloc::copy(const size_t tac_index, const size_t lval_tac, const size_t rval_tac)
	{
		REG lval_reg = this->get(lval_tac);
		REG rval_reg = this->get(rval_tac);

		const DataType ldt = m_registers[static_cast<size_t>(lval_reg)].data_type;
		const DataType rdt = m_registers[static_cast<size_t>(rval_reg)].data_type;

		if (rdt == DataType::POINTER)
		{
			rval_reg = this->loadPointer(rval_tac, rval_reg);
			lval_reg = this->get(lval_tac);
		}

		if (ldt == DataType::POINTER)
		{
			REG temp = this->allocTemporary(tac_index);
			this->swap(temp);
			rval_reg = this->get(rval_tac);
			lval_reg = this->get(lval_tac);
			m_asm_table->add<ins::MOVE>(rval_reg);
			m_asm_table->add<ins::RSTORE>(lval_reg);
		}
		else if (ldt == DataType::VARIABLE)
		{
			// lval_reg = this->swap(lval_reg);
			// rval_reg = this->getValue(rval_tac);
			// m_asm_table->add<ins::MOVE>(rval_reg);
			this->swap(rval_reg);
			lval_reg = this->get(lval_tac);
			m_asm_table->add<ins::STORE>(m_registers[static_cast<size_t>(lval_reg)].address);
		}
		else if (ldt == DataType::TEMPORARY)
		{
			lval_reg = this->swap(lval_reg);
			m_asm_table->add<ins::MOVE>(rval_reg);
			m_registers[0].address = 0;
			m_registers[0].data_type = DataType::TEMPORARY;
			m_registers[0].tac = tac_index;
			// panic("illegal operation - rvalue assignment; value-type={}", static_cast<size_t>(ldt));
		}

		// this->swap(rval_reg);
		// rval_reg = REG::RA;
		// lval_reg = get(lval_tac);
		// if (ldt == DataType::POINTER)
		// 	m_asm_table->add<ins::RSTORE>(lval_reg);
		// else
		// 	m_asm_table->add<ins::STORE>(m_registers[static_cast<size_t>(lval_reg)].address);

		// this->swap(lval_reg);
		// lval_reg = REG::RA;
		// rval_reg = get(rval_tac);
		// if (m_registers[static_cast<size_t>(lval_reg)].data_type == DataType::POINTER)
		// { /* do nothing */
		// }
		// else
		// 	m_asm_table->add<ins::LOAD>(m_registers[static_cast<size_t>(lval_reg)].address);

		// if (m_tac_info->at(rval_tac).hasNextUse(tac_index))
		// {
		// 	// LD/ST of shame
		// 	this->swap(rval_reg);
		// 	m_asm_table->add<ins::STORE>(m_registers[static_cast<size_t>(lval)].address);
		// 	this->swap(lval);
		// 	m_asm_table->add<ins::LOAD>(m_registers[static_cast<size_t>(lval)].address);
		// }
		// else
		// {
		// 	this->swap(lval);

		// 	// no next use --> swap the registers (internally)

		// 	// reset lval register (will become an empty rval_reg)
		// 	m_registers[static_cast<size_t>(rval_reg)].tac = empty_tac;
		// 	m_registers[static_cast<size_t>(rval_reg)].address = 0;
		// 	m_registers[static_cast<size_t>(rval_reg)].data_type = DataType::NONE;

		// 	// std::swap(m_registers[static_cast<size_t>(lval)], m_registers[static_cast<size_t>(rval_reg)]);
		// 	m_asm_table->add<ins::SWP>(rval_reg);
		// }
	}

    void RegAlloc::flushTemporaryTAC()
    {
		for (size_t i = 0; i < m_registers.size(); i++)
		{
			for (const size_t t : temp_tac)
			{
				if (m_registers[i].tac == t)
				{
					this->resetRegister(static_cast<REG>(i));
				}
			}
		}
    }

	void RegAlloc::__debug_print() const
	{
		std::println("reg contents:");
		for (size_t i = 0; i < m_registers.size(); i++)
		{
			std::println("{}: code=({}), var-type='{}', addr={}", static_cast<REG>(i), m_registers[i].tac, static_cast<int>(m_registers[i].data_type), m_registers[i].address);
		}
	}

	REG RegAlloc::getEmptyRegister(const size_t tac, const size_t addr)
	{
		// if (addr > 0) // not null
		// {
		// 	// search for a register containing this address
		// 	for (size_t i = 0; i < m_registers.size(); i++)
		// 	{
		// 		if (m_registers[i].address == addr)
		// 		{
		// 			return static_cast<REG>(i);
		// 		}
		// 	}
		// }

		// search for empty registers
		for (size_t i = 0; i < m_registers.size(); i++)
		{
			if (m_registers[i].data_type == DataType::NONE)
			{
				m_registers[i].tac = empty_tac; // redundant
				m_registers[i].address = 0;
				return static_cast<REG>(i);
			}
		}

		// search for dead registers
		for (size_t i = 0; i < m_registers.size(); i++)
		{
			size_t reg_tac = m_registers[i].tac;
			if (reg_tac >= m_tac_info->size()) panic("internal compiler error: empty tac found after empty tac search (wft)");

			if (!m_tac_info->at(reg_tac).hasNextUse(tac))
			{
				this->resetRegister(static_cast<REG>(i));
				return static_cast<REG>(i);
			}
		}

		warning("no empty register found - putting on stack");

		// WARNING: stuff below not tested properly

		// generate store call on a random [RB,RH] register
		std::mt19937_64 gen(0);
		std::uniform_int_distribution<size_t> dist(1, m_registers.size());
		const size_t reg = dist(gen);

		this->swap(static_cast<REG>(reg));
		size_t next_stack_addr = m_stack_ptr + m_stack.size();
		m_asm_table->add<ins::STORE>(next_stack_addr);
		m_stack.push_back(m_registers[0]); // we swapped this reg to RA
		this->swap(static_cast<REG>(reg)); // swap back RA

		return static_cast<REG>(reg);

	}

} // namespace fl

