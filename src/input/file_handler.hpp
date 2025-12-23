#pragma once

#include <filesystem>


namespace fl
{

	class FileHandler
	{
	public:

		FILE* const handler;

		FileHandler(const std::filesystem::path& path);
		~FileHandler();

	};

} // namespace fl