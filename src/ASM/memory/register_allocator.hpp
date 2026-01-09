#pragma once

#include <array>
#include <vector>
#include <memory>
#include <limits>

#include "../../TAC/tac_info.hpp"
#include "address_descriptor.hpp"


namespace fl
{

	class RegAlloc
	{
	private:

		constexpr static size_t empty_tac = std::numeric_limits<size_t>::max();

		struct Register
		{
			size_t tac_index {empty_tac};
			std::string assoc_variable;
		};

		std::shared_ptr<std::vector<TACInfo>> m_tac_info;
		std::array<Register, __reg_count> m_registers;
		AddrDescriptor m_addr_descriptor;

		const size_t m_stack_ptr;

	public:

		RegAlloc(std::shared_ptr<std::vector<TACInfo>> tac_info, const size_t stack_ptr);
		~RegAlloc() = default;

		void allocate(const size_t tac);
		void allocate(const size_t tac, const std::string_view variable_name);

		// swaps 'reg' with RA
		void swap(const REG reg);

		REG get(const size_t tac);
	};

} // namespace fl
