#pragma once

#include "../instruction.hpp"
#include "../memory/register.hpp"


namespace fl::ins
{

	class LABEL : public Instruction
	{
	public:

		const size_t id;

	public:

		LABEL(const size_t label_id)
			: id(label_id)
		{}

		~LABEL() = default;

		constexpr virtual size_t size() const override
		{
			return 0;
		}

		std::string generate() const
		{
			// return std::format("# LABEL {}", id);
			return {};
		}

	};
	
} // namespace fl::ins
