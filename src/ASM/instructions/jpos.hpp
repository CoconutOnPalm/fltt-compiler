#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class JPOS : public Instruction
	{
	private:

		size_t pos;

	public:

		JPOS(const size_t _pos)
			: pos(_pos)
		{
		}

		~JPOS() = default;

		std::string generate() const
		{
			return std::format("JPOS {}", pos);
		}

		virtual void updateJumpDestination(const std::unordered_map<size_t, size_t> label_map) override
		{
			if (!label_map.contains(pos))
				panic("internal compiler error: JPOS jump update - label (id={}) not found", pos);

			pos = label_map.at(pos);
		}

	};

} // namespace fl::ins
