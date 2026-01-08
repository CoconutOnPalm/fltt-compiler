#pragma once


#include <memory>

#include "../../tac.hpp"
#include "../other/label.hpp"


namespace fl::tac
{

	class JZ : public TAC
	{
	private:

		const size_t condition_index;
		std::shared_ptr<size_t> label_id;
		
	public:

		JZ(const size_t cond_index, std::shared_ptr<size_t> label)
			: condition_index(cond_index), label_id(label)
		{}
		virtual ~JZ() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::JUMP);
		}

		void updateNextUse(std::vector<TACInfo>& info_table) const override
		{
			info_table[condition_index].useIn(p_index);
		}

		virtual void generateASM() const override
		{
			std::println("{}", __debug_string());
		}

		virtual std::string __debug_string() const override
		{
			return std::format("JZ ({}) --> {}", condition_index, *(label_id.get()));
		}
	};
	
} // namespace fl::tac
