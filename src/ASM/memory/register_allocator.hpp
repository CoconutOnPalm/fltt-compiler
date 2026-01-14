#pragma once

#include <array>
#include <vector>
#include <memory>
#include <limits>
#include <set>

#include "../../TAC/tac_info.hpp"
#include "address_descriptor.hpp"
#include "../asm_table.hpp"
#include "../../symbol/symbol_table.hpp"


namespace fl
{

	class RegAlloc
	{
	private:

		constexpr inline static size_t empty_tac = -1;

		enum class DataType
		{
			NONE,
			VARIABLE,	// variable identified by 
			POINTER,
			IMMEDIATE,	// loaded immediate
			TEMPORARY,	// compiler-generated temporary
		};

		struct Register
		{
			size_t tac { empty_tac };
			DataType data_type { DataType::NONE };
			size_t address { 0 };
		};
		
		std::array<Register, __reg_count> m_registers;
		const size_t m_stack_ptr;
		std::vector<Register> m_stack;


		std::shared_ptr<std::vector<TACInfo>> m_tac_info;
		std::shared_ptr<ASMTable> m_asm_table;
		

	public:

		RegAlloc(std::shared_ptr<std::vector<TACInfo>> tac_info, std::shared_ptr<ASMTable> asm_table, const size_t stack_ptr);
		~RegAlloc() = default;

		// void allocate(const size_t tac);
		// void allocate(const size_t tac, const std::string_view variable_name);

		// void assign(const REG reg, const size_t tac);

		void updateRA(const size_t tac);

		// swaps 'reg' with RA
		void swap(const REG reg);

		REG allocPointer(const size_t tac);
		REG allocImmediate(const size_t tac);
		REG allocTemporary(const size_t tac);

		REG loadVariable(const size_t tac, const size_t addr);
		REG loadPointer(const size_t tac, REG reg);
		REG loadImmediate(const size_t tac, const uint64_t imm);
		
		REG get(const size_t tac);
		REG getValue(const size_t tac);

		void resetRegister(const REG reg);

		void copy(const size_t tac_index, const size_t lval_tac, const size_t rval_tac);

		void __debug_print() const;

	private:

		REG getEmptyRegister(const size_t tac, const size_t addr = 0);
	};

} // namespace fl
