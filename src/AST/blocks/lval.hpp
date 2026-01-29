#pragma once

#include "identifier.hpp"
#include "../../TAC/codes/other/LDU.hpp"


namespace fl::ast
{

	class LvalIdentifier : public Identifier
	{
	public:

		// inherited from Identifier

	public:

		LvalIdentifier(const std::string_view id) : Identifier(id) {}
		virtual ~LvalIdentifier() = default;

		virtual size_t generateTAC(TACTable& tac_table) const override
		{
			return tac_table.add<tac::LDU>(identifier, p_owner);
		}
	};

} // namespace fl::ast