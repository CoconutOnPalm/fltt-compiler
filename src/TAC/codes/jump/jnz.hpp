#pragma once


#include <memory>

#include "../../tac.hpp"
#include "../other/label.hpp"


namespace fl::tac
{

	class JNZ : public TAC
	{
	private:

		const size_t condition_index;
		std::shared_ptr<size_t> label_id;
		
	public:

		JNZ(const size_t cond_index, std::shared_ptr<size_t> label, const std::string_view owning_proc)
			: TAC(owning_proc), condition_index(cond_index), label_id(label)
		{}
		virtual ~JNZ() = default;


		TACInfo getSelfInfo() const
		{
			return TACInfo(TACType::JUMP, p_owning_procedure);
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
			return std::format("JNZ ({}) --> {}", condition_index, *(label_id.get()));
		}
	};
	
} // namespace fl::tac
