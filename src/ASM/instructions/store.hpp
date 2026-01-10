#pragma once

#include "../instruction.hpp"


namespace fl::ins
{
	
	class WRITE : public Instruction
	{
	private:

		const size_t addr;

	public:

		WRITE(const size_t mem_addr)
			: addr(mem_addr)
		{}
		virtual ~WRITE() = default;

		std::string generate() const
		{
			return std::format("WRITE {}", addr);
		}
	};
	
} // namespace fl::asm
