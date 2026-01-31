#include "arg_parser.hpp"

#include <string>
#include <argparse/argparse.hpp>

#include "../utils/panic.hpp"
#include "../utils/const.hpp"


namespace fl
{
	
    ProgramArgs parse_args(const int argc, const char* const* argv)
    {
		argparse::ArgumentParser parser("kompilator", "1.0");
		
		parser.add_argument<std::string>("src-file")
			.help(".imp source file")
			.required();
		parser.add_argument<std::string>("asm-file")
			.help(".mr vm-assembly file (compiler output)")
			.required();
		
		try {
			parser.parse_args(argc, argv);
		} catch (const std::exception& e) {
			std::println(std::cerr, "{}", e.what());
			std::exit(1);
		}

		return ProgramArgs {
			.language_path{parser.get<std::string>("src-file")},
			.machine_code_path{parser.get<std::string>("asm-file")},
		};
    }

} // namespace fl
