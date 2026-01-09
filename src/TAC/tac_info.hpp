#pragma once

#include <string>
#include <queue>


namespace fl
{
	enum class TACType
	{
		VARIABLE,
		CONSTANT,
		ARRELEM,
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

		const std::string associated_variable {};
		const std::string owning_proc {};
		const TACType code_type;
		const bool block_terminator;

	private:

		std::vector<size_t> m_usages;
	
	public:

		TACInfo(const TACType code_type, const std::string_view owning_procedure, const std::string_view assoc_var = "")
			: code_type(code_type), owning_proc(owning_procedure), block_terminator(mapBlockTerminators(code_type)), associated_variable(assoc_var)
		{}
		~TACInfo() = default;

		void useIn(const size_t tac_index);
		size_t nextUsage(const size_t current_tac_index);
		bool hasNextUse(const size_t current_tac_index) const;
		std::vector<size_t> usages() const;

	private:

		static bool mapBlockTerminators(const TACType type);
	};
	
} // namespace fl
