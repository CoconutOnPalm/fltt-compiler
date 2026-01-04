#pragma once


#include <string>


namespace fl
{
	
	class TAC
	{
	protected:

	public:

		TAC() = default;
		virtual ~TAC() = default;

		virtual void generateASM() const = 0;

		virtual std::string __debug_string() const = 0;
	};
	
} // namespace fl
