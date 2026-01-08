#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Read : public TAC
	{
	private:

		const size_t lvalue;

	public:

		Read(const size_t lval)
			: lvalue(lval)
		{}

		virtual ~Read() = default;

		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::IO);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[lvalue].useIn(p_index);
		}
		
		virtual void generateASM() const 
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const
		{
			return std::format("READ ({})", lvalue);
		}
	};
	
} // namespace fl
