#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class CALL : public Instruction
	{
	private:

		size_t pos;

	public:

		CALL(const size_t _pos)
			: pos(_pos)
		{}

		~CALL() = default;

		std::string generate() const
		{
			return std::format("CALL {}", pos);
		}

		virtual void updateJumpDestination(const std::unordered_map<size_t, size_t> label_map) override
		{
			if (!label_map.contains(pos))
				panic("internal compiler error: JZERO jump update - label (id={}) not found", pos);

			pos = label_map.at(pos);
		}
	};
	
} // namespace fl::ins
