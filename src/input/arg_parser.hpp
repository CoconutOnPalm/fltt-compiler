#pragma once

#include <filesystem>


namespace fl
{
	
	struct ProgramArgs
	{
		std::filesystem::path language_path;		// in
		std::filesystem::path machine_code_path;	// out
	};
	
	ProgramArgs parse_args(const int argc, const char* const* argv);

} // namespace fl
