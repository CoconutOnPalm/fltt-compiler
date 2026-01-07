#pragma once


#include <memory>

#include "../../tac.hpp"
#include "../other/label.hpp"


namespace fl::tac
{

	class JUMP : public TAC
	{
	private:

		std::shared_ptr<size_t> label_id;
		
	public:

		JUMP(std::shared_ptr<size_t> label)
			: label_id(label)
		{}
		virtual ~JUMP() = default;

		virtual void generateASM() const override
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("JUMP --> {}", *(label_id.get()));
		}
	};
	
} // namespace fl::tac
