#pragma once

#include <print>
#include <iostream>

extern int yylineno;

namespace fl
{
	template <typename... Args>
	void warning(const std::format_string<Args...> format_str, Args&&... args)
	{
		std::print("\033[33m[WARN]\033[0m: ");
		std::println(std::cerr, format_str, std::forward<Args>(args)...);
	}

} // namespace fl