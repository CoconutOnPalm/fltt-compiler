#pragma once

#include "../instruction.hpp"


namespace fl::ins
{
	
	class LOAD : public Instruction
	{
	private:

		const size_t addr;

	public:

		LOAD(const size_t mem_addr)
			: addr(mem_addr)
		{}
		virtual ~LOAD() = default;

		std::string generate() const
		{
			return std::format("LOAD {}", addr);
		}
	};
	
} // namespace fl::asm
