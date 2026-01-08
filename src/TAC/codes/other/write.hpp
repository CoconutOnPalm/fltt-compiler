#pragma once

#pragma once

#include <cstdint>
#include <print>
#include <format>

#include "../../tac.hpp"


namespace fl::tac
{
	
	class Write : public TAC
	{
	private:

		const size_t rvalue;

	public:

		Write(const size_t rval)
			: rvalue(rval)
		{}

		virtual ~Write() = default;

		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::IO);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[rvalue].useIn(p_index);
		}
		
		virtual void generateASM() const 
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const
		{
			return std::format("WRITE ({})", rvalue);
		}
	};
	
} // namespace fl
