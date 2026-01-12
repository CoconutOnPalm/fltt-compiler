#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Assign : public TAC
	{

		size_t lval;
		size_t rval;

	public:

		Assign(const size_t lvalue, const size_t rvalue, const std::string_view owning_proc)
			: TAC(owning_proc), lval(lvalue), rval(rvalue)
		{}

		virtual ~Assign() = default;

		virtual TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::ASSIGNMENT, p_owning_procedure);
		}

		virtual void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[lval].useIn(p_index);
			info_table[rval].useIn(p_index);
		}

		virtual void typeCheck(const std::vector<TACInfo>& info_table, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			TACType ct = info_table[lval].code_type;
			if (ct != TACType::VARIABLE && ct != TACType::ARRELEM)
				panic("illegal operation - rvalue assignment ({})", static_cast<int>(info_table[lval].code_type));

			std::string var = info_table[lval].associated_variable;
			if (var.empty()) // array indexer
				return;

			SymbolTable& symbol_tab = *(symbol_tables[p_owning_procedure].get());
			const Symbol& symbol = symbol_tab[var];
			if (symbol.testFlag(SymbolType::IN))
				panic("illegal operation - assignment to a const variable");

		}

		virtual void generateASM(ASMTable& asm_table, RegAlloc& regalloc, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			std::println("{:2}| lval={}, rval={}", p_index, regalloc.get(lval), rval);
			regalloc.copy(lval, rval, p_index);
		}

		virtual std::string __debug_string() const override
		{
			return std::format("({}) = ({})", lval, rval);
		}
	};


	class ForceAssign : public Assign
	{
	public:

		ForceAssign(const size_t lvalue, const size_t rvalue, const std::string_view owning_proc)
			: Assign(lvalue, rvalue, owning_proc)
		{}

		virtual ~ForceAssign() = default;

		void typeCheck(const std::vector<TACInfo>& info_table, std::map<std::string, std::shared_ptr<SymbolTable>>& symbol_tables) const override
		{
			/* override so the assignment doesn't type check */
		}
	};

	
} // namespace fl
