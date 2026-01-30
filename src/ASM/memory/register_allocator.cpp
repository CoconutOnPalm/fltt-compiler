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

	void RegAlloc::overrideRA(const Register& regdata)
	{
		m_registers[0] = regdata;
	}

	void RegAlloc::overrideRA(const size_t tac)
	{
		m_registers[0].data_type = DataType::TEMPORARY;
		m_registers[0].address = 0;
		m_registers[0].tac = tac;
	}

	void RegAlloc::overrideRegister(const REG reg, const Register& regdata)
	{
		m_registers[static_cast<size_t>(reg)] = regdata;
	}

	void RegAlloc::overrideRegister(const REG reg, const size_t tac)
	{
		m_registers[static_cast<size_t>(reg)].data_type = DataType::TEMPORARY;
		m_registers[static_cast<size_t>(reg)].address = 0;
		m_registers[static_cast<size_t>(reg)].tac = tac;
	}

    void RegAlloc::flushRegisters()
    {
		// reset all registers
		for (size_t i = 0; i < __reg_count; i++)
		{
			this->resetRegister(static_cast<REG>(i));
		}
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
		const REG reg = this->getEmptyRegister(tac);
		m_registers[static_cast<size_t>(reg)].data_type = DataType::POINTER;
		m_registers[static_cast<size_t>(reg)].tac = tac;
		m_registers[static_cast<size_t>(reg)].address = 0;
		return reg;
	}

	REG RegAlloc::allocImmediate(const size_t tac)
	{
		const REG reg = this->getEmptyRegister(tac);
		m_registers[static_cast<size_t>(reg)].data_type = DataType::IMMEDIATE;
		m_registers[static_cast<size_t>(reg)].tac = tac;
		m_registers[static_cast<size_t>(reg)].address = 0;
		return reg;
	}

	REG RegAlloc::allocTemporary(const size_t tac)
	{
		const REG reg = this->getEmptyRegister(tac);
		m_registers[static_cast<size_t>(reg)].data_type = DataType::TEMPORARY;
		m_registers[static_cast<size_t>(reg)].tac = tac;
		m_registers[static_cast<size_t>(reg)].address = 0;
		return reg;
	}

	REG RegAlloc::allocVariable(const size_t tac, const uint64_t address)
	{
		const REG reg = this->getEmptyRegister(tac);
		m_registers[static_cast<size_t>(reg)].data_type = DataType::VARIABLE;
		m_registers[static_cast<size_t>(reg)].tac = tac;
		m_registers[static_cast<size_t>(reg)].address = address;
		return reg;
	}

	REG RegAlloc::loadVariable(const size_t tac, const size_t addr, bool use_reg_search)
	{
		if (addr == 0)
			panic("null ptr dereference");

		if (use_reg_search)
		{
			// search through the registers to find pre-loaded variable
			for (size_t i = 0; i < m_registers.size(); i++)
			{
				if (m_registers[i].address == addr)
				{
					REG copee = static_cast<REG>(i);
					REG val_reg = this->getEmptyRegister(tac);
					const REG reg_position = val_reg;

					if (copee == val_reg)
						continue; // getEmptyRegister() killed copee

					if (copee == REG::RA) // prevent register loss
						copee = val_reg;

					this->swap(val_reg); // RA = ptr_reg

					m_asm_table->add<ins::MOVE>(copee);

					m_registers[0].data_type = DataType::VARIABLE;
					m_registers[0].tac = tac;
					m_registers[0].address = addr;

					this->swap(reg_position);

					return reg_position;
				}
			}
		}

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

	REG RegAlloc::storeVariable(REG reg, const size_t addr, const size_t stored_var_tac)
	{
		reg = this->swap(reg);
		m_asm_table->add<ins::STORE>(addr);
		this->overrideRA(Register{
				.tac = stored_var_tac,
				.data_type = DataType::VARIABLE,
				.address = addr,
			});

		// destroy all other occurances of lval
		for (size_t i = 1; i < m_registers.size(); i++)
		{
			if (m_registers[i].address == addr)
				this->resetRegister(static_cast<REG>(i));
		}

		return reg; // == RA
	}

	// warning - this destroys whatever's inside 'reg'
	REG RegAlloc::loadPointer(const size_t tac, REG reg)
	{
		REG val_reg = this->getEmptyRegister(tac);
		const REG reg_position = val_reg;

		this->resetRegister(reg); // TODO: remove

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
		REG imm_reg = this->getEmptyRegister(tac);

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
		panic("register lost for TAC ({})", tac);

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
			const uint64_t address = m_registers[static_cast<size_t>(lval_reg)].address;
			// lval_reg = this->swap(lval_reg);
			// rval_reg = this->getValue(rval_tac);
			// m_asm_table->add<ins::MOVE>(rval_reg);
			rval_reg = this->swap(rval_reg);
			lval_reg = this->get(lval_tac);
			m_asm_table->add<ins::STORE>(address);
			// lval_reg = this->swap(lval_reg);

			this->overrideRA(Register{
				.tac = lval_tac,
				.data_type = DataType::VARIABLE,
				.address = address,
				});

			// destroy all other occurances of lval
			for (size_t i = 1; i < m_registers.size(); i++)
			{
				if (m_registers[i].address == address)
					this->resetRegister(static_cast<REG>(i));
			}
		}
		else if (ldt == DataType::TEMPORARY)
		{
			lval_reg = this->swap(lval_reg);
			// rval_reg = this->get(rval_tac);
			m_asm_table->add<ins::MOVE>(rval_reg);
			// m_registers[0].address = m_registers[static_cast<size_t>(lval_reg)].address;
			// m_registers[0].data_type = DataType::VARIABLE;
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

	REG RegAlloc::getEmptyRegister(size_t tac, const size_t relative_tac)
	{
		if (this->isTemporary(tac))
		{
			// make the search relative to the largest non-temp tac 
			size_t max_tac = 0;
			for (size_t i = 0; i < m_registers.size(); i++)
			{
				if (!this->isTemporary(m_registers[i].tac))
				{
					max_tac = std::max(max_tac, m_registers[i].tac);
				}
			}

			tac = max_tac;
		}

		// if (tac >= m_tac_info->size())
		// {
		// 	// delete register with the least tac (NOT TESTED IF THIS WROKS)
		// 	size_t min_tac = -1;
		// 	REG min_reg = REG::RH;
		// 	for (size_t i = 0; i < m_registers.size(); i++)
		// 	{
		// 		if (m_registers[i].tac < min_tac)
		// 		{
		// 			min_tac = m_registers[i].tac;
		// 			min_reg = static_cast<REG>(i);
		// 		}
		// 	}

		// 	this->resetRegister(min_reg);
		// 	return min_reg;
		// }

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
		// (1) try not to alloc variables
		for (size_t i = 0; i < m_registers.size(); i++)
		{
			size_t reg_tac = m_registers[i].tac;

			if (this->isTemporary(reg_tac))
				continue;

			// if (m_registers[i].address > 0)
			// 	continue;

			// std::println("[debug]: reg_tac={}", reg_tac);
			if (!m_tac_info->at(reg_tac).hasNextUse(tac))
			{
				this->resetRegister(static_cast<REG>(i));
				return static_cast<REG>(i);
			}
		}


		static std::mt19937_64 gen(0); // seed = 0
		static std::uniform_int_distribution<size_t> dist(0, m_registers.size() - 1);
		
		// (2) all registers are variables - choose one randomly
		// max 50 attempts
		// for (size_t _ = 0; _ < 50; _++)
		// {
		// 	size_t rand_i = dist(gen);
		// 	const REG reg = static_cast<REG>(rand_i);
		// 	size_t reg_tac = m_registers[rand_i].tac;

		// 	if (this->isTemporary(reg_tac))
		// 		continue;

		// 	if (!m_tac_info->at(reg_tac).hasNextUse(tac))
		// 	{
		// 		this->resetRegister(reg);
		// 		return reg;
		// 	}
		// }

		// (3) somehow the probablility god is not on our side - kill the first empty one
		// for (size_t i = 0; i < m_registers.size(); i++)
		// {
		// 	size_t reg_tac = m_registers[i].tac;

		// 	if (this->isTemporary(reg_tac))
		// 		continue;

		// 	// std::println("[debug]: reg_tac={}", reg_tac);
		// 	if (!m_tac_info->at(reg_tac).hasNextUse(tac))
		// 	{
		// 		this->resetRegister(static_cast<REG>(i));
		// 		return static_cast<REG>(i);
		// 	}
		// }

		warning("no empty register found - putting on stack");

		// WARNING: stuff below not tested properly

		// generate store call on a random [RB,RH] register
		// std::mt19937_64 gen(0);
		// std::uniform_int_distribution<size_t> dist(1, m_registers.size());
		// const size_t reg = dist(gen);

		// this->swap(static_cast<REG>(reg));
		// size_t next_stack_addr = m_stack_ptr + m_stack.size();
		// m_asm_table->add<ins::STORE>(next_stack_addr);
		// m_stack.push_back(m_registers[0]); // we swapped this reg to RA
		// this->swap(static_cast<REG>(reg)); // swap back RA

		return REG::RA;

	}

	bool RegAlloc::isTemporary(const size_t tac) const
	{
		for (const size_t temp : temp_tac)
		{
			if (tac == temp)
				return true;
		}

		return false;
	}

} // namespace fl

