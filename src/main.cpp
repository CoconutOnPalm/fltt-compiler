#include <print>

#include "input/arg_parser.hpp"
#include "input/file_handler.hpp"


extern void run_parser(FILE* in_str);


int main(const int argc, const char* const* argv)
{
	fl::ProgramArgs args = fl::parse_args(argc, argv);
	fl::FileHandler file_handler(args.language_path);

	std::println("compiling {}", args.language_path.string());
	run_parser(file_handler.handler);
}