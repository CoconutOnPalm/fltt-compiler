#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{
	
	class RSTORE : public Instruction
	{
	private:

		const REG reg;

	public:

		RSTORE(const REG _reg)
			: reg(_reg)
		{}
		virtual ~RSTORE() = default;

		std::string generate() const
		{
			return std::format("RSTORE {}", reg);
		}
	};
	
} // namespace fl::ins
