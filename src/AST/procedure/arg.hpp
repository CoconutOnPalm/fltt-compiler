#pragma once

#include <format>
#include <map>


namespace fl
{
	
	enum class ArgType
	{
		IN,
		OUT,
		ARRAY,
		NONE,
	};


	class Argument
	{
	private:

		const ArgType type;
		std::string identifier;

	public:

		Argument(const ArgType argtype, const std::string& identifier)
			: type(argtype), identifier(identifier)
		{}

	};


} // namespace fl


template <typename char_T>
struct std::formatter<fl::ArgType, char_T> : std::formatter<std::string>
{
	auto format(const fl::ArgType& arg_type, std::format_context& ctx) const
	{
		const static std::map<fl::ArgType, std::string> mapper = {
			{ fl::ArgType::IN, 		"I" },
			{ fl::ArgType::OUT, 	"O" },
			{ fl::ArgType::ARRAY, 	"T" },
			{ fl::ArgType::NONE, 	{ } },
		}; 
		return std::formatter<std::string>::format(std::format("{}", mapper.at(arg_type)), ctx);
	}
};