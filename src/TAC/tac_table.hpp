#pragma once

#include <vector>
#include <memory>

#include "tac.hpp"

namespace fl
{

	class TACTable
	{
	private:

		std::vector<std::unique_ptr<TAC>> m_tac_table;

	public:

		TACTable() = default;
		~TACTable() = default;
	
		template <typename CT, typename... Args>
		size_t add(Args&&... args)
		{
			m_tac_table.emplace_back(std::make_unique<CT>(std::forward<Args>(args)...));
			return m_tac_table.size() - 1;
		}

		void generateASM() const;
	};
	
} // namespace fl
