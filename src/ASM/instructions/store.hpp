#pragma once

#include "../instruction.hpp"


namespace fl::ins
{
	
	class STORE : public Instruction
	{
	private:

		const size_t addr;

	public:

		STORE(const size_t mem_addr)
			: addr(mem_addr)
		{}
		virtual ~STORE() = default;

		std::string generate() const
		{
			return std::format("STORE {}", addr);
		}
	};
	
} // namespace fl::ins
