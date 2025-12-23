#include "file_handler.hpp"

#include "../utils/panic.hpp"


namespace fl
{
	
	FileHandler::FileHandler(const std::filesystem::path& path)
	: handler(fopen64(path.c_str(), "r"))
	{
		if (handler == nullptr)
		{
			// try to determine the cause
			std::string cause {};
			if (!std::filesystem::exists(path)) cause = "file does not exist";

			panic("could not open '{}': {}", path.string(), cause);
		}
	}
	
	FileHandler::~FileHandler()
	{
		if (handler != nullptr)
			fclose(handler);
	}
	
} // namespace fl