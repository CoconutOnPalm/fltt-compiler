#pragma once

#include <memory>
#include <vector>
#include <fstream>
#include <unordered_map>

#include "instruction.hpp"
#include "instructions/label.hpp"


namespace fl
{

	class ASMTable
	{
	private:

		std::vector<std::unique_ptr<Instruction>> m_instructions;

		std::unordered_map<size_t, size_t> m_labels; // <id, position>

	public:

		ASMTable() = default;
		~ASMTable() = default;



		template <typename InstrT, typename... Args>
		void add(Args&&... args)
		{
			if constexpr (std::is_same_v<InstrT, ins::LABEL>)
			{
				static_assert(sizeof...(Args) == 1, "too many arguments given to the ins::LABEL construtor");

				const size_t label_id = static_cast<size_t>((args, ...));

				if (m_labels.contains(label_id))
					panic("redeclaration of a label (id={})", label_id);

				m_labels[label_id] = -1;
			}

			m_instructions.emplace_back(std::make_unique<InstrT>(std::forward<Args>(args)...));
		}

		void generate(std::ofstream& ofstr);

	private:

		void updateInstructionIndicies();
		void updateJumpPositions();

	};

} // namespace fl
