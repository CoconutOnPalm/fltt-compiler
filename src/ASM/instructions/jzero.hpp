#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class JZERO : public Instruction
	{
	private:

		size_t pos;

	public:

		JZERO(const size_t label_id)
			: pos(label_id)
		{}

		~JZERO() = default;

		std::string generate() const
		{
			return std::format("JZERO {}", pos);
		}

		virtual void updateJumpDestination(const std::unordered_map<size_t, size_t> label_map) override
		{
			if (!label_map.contains(pos))
				panic("internal compiler error: JZERO jump update - label (id={}) not found", pos);

			pos = label_map.at(pos);
		}
	};
	
} // namespace fl::ins
