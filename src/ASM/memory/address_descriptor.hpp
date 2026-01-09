#pragma once

#include <set>
#include <map>
#include <string>

#include "register.hpp"


namespace fl
{
	
	class AddrDescriptor
	{
	private:

		struct Addr
		{
			bool addr_updated; // is var's physical address up to date?
			std::set<REG> registers;
		};

		std::map<std::string, Addr> m_addr_descriptor;

	public:

		AddrDescriptor() = default;
		~AddrDescriptor() = default;

		void triggerLoad(const std::string_view variable_name, const REG reg);
		void triggerStore(const std::string_view variable_name);

	};
	
} // namespace fl
