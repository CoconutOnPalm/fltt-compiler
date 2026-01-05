#pragma once

#include <cstdint>
#include <print>
#include <format>
#include <memory>

#include "../tac.hpp"


namespace fl::tac
{
	
	class Label : public TAC
	{
	private:

		inline static uint64_t uuid_selector { 0 };

		const std::string name;
		std::shared_ptr<uint64_t> uuid;

	public:

		Label(const std::string_view _name, std::shared_ptr<uint64_t> uuid)
			: name(_name), uuid(uuid)
		{ *uuid = nextUUID(); }
		~Label() = default;

		virtual void generateASM() const override
		{
			std::println("# {}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("{} [{}]", name, *uuid);
		}


		static uint64_t nextUUID()
		{
			return uuid_selector++;
		}

	};
	
} // namespace fl
