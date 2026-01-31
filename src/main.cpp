#include <print>

#include "input/arg_parser.hpp"
#include "input/file_handler.hpp"
#include "compiler.hpp"


extern void run_parser(fl::Compiler& compiler, FILE* in_str);


int main(const int argc, const char* const* argv)
{
	fl::Compiler compiler;

	fl::ProgramArgs args = fl::parse_args(argc, argv);
	fl::FileHandler file_handler(args.language_path);

	run_parser(compiler, file_handler.handler);

	compiler.compile(args.machine_code_path);
}