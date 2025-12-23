#pragma once

#include <print>
#include <iostream>


namespace fl
{
	template <typename... Args>
	void panic(const std::format_string<Args...> format_str, Args&&... args)
	{
		std::print("\033[31m[ERROR]\033[0m ");
		std::println(std::cerr, format_str, std::forward<Args>(args)...);
		std::exit(1);
	}

} // namespace fl