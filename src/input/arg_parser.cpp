#include "arg_parser.hpp"

#include <string>

#include "../utils/panic.hpp"


namespace fl
{
	template <typename... Args>
	void argparser_error(const std::format_string<Args...> format_str, Args&&... args)
	{
		std::print("\033[31m[ERROR]\033[0m ");
		std::println(std::cerr, format_str, std::forward<Args>(args)...);
		std::println("usage: kompilator <source-code-path> <machine-code-path>");
		std::exit(1);
	}
	
    ProgramArgs parse_args(const int argc, const char* const* argv)
    {
		if (argc != 3) // {program, lang_src, mc_src}
			argparser_error("invalid argument count; expected 2, got {}", argc - 1);

		const std::string lang_src_path(argv[1]);
		const std::string machine_code_path(argv[1]);

        return ProgramArgs{
			.language_path{lang_src_path},
			.machine_code_path{machine_code_path}
		};
    }

} // namespace fl
