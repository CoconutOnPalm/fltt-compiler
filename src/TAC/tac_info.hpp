#pragma once


#include <queue>


namespace fl
{
	enum class TACType
	{
		VARIABLE,
		CONSTANT,
		EXPRESSION,
		CONDITION,
		JUMP,
		ASSIGNMENT,
		PARAM,
		LABEL,
		IO,
	};

	class TACInfo
	{
	public: 

		const TACType code_type;
		const bool block_terminator;

	private:

		std::queue<size_t> m_next_use;
	
	public:

		TACInfo(const TACType code_type)
			: code_type(code_type), block_terminator(mapBlockTerminators(code_type))
		{}
		~TACInfo() = default;

		void useIn(const size_t tac_index);
		size_t nextUsage();
		bool hasNextUse() const;

	private:

		static bool mapBlockTerminators(const TACType type);
	};
	
} // namespace fl
