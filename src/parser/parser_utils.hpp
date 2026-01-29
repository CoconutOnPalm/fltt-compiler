#pragma once

#include <variant>

#include "../utils/const.hpp"

#include "../procedure/procedure_tree.hpp"

#include "../AST/syntax_tree.hpp"
#include "../AST/blocks/block.hpp"
#include "../AST/blocks/lval.hpp"
#include "../AST/blocks/expressions/addition.hpp"
#include "../AST/blocks/expressions/substraction.hpp"
#include "../AST/blocks/expressions/multiplication.hpp"
#include "../AST/blocks/expressions/division.hpp"
#include "../AST/blocks/expressions/modulo.hpp"
#include "../AST/blocks/indexof.hpp"
#include "../AST/blocks/assign.hpp"
#include "../AST/blocks/if.hpp"
#include "../AST/blocks/ifelse.hpp"
#include "../AST/blocks/while.hpp"
#include "../AST/blocks/dowhile.hpp"
#include "../AST/blocks/for.hpp"
#include "../AST/blocks/proc_decl.hpp"
#include "../AST/blocks/proc_call.hpp"
#include "../AST/blocks/io.hpp"


namespace fl::parser
{
	
	// enum TACNodeType : int
    // {
    //     NUM,
    //     IDENTIFIER,
    //     TAC_INDEX,
    //     NONE,
    // };

	// template <typename YYT>
	// void assignValueVariant(fl::tacval_t& val, const YYT& expr_val)
    // {
    // 	switch (expr_val.type) {
	// 	case fl::parser::TACNodeType::IDENTIFIER:
	// 		std::println("type=identifier, value='{}'", expr_val.id);
	// 		val.emplace<0>(expr_val.id);
	// 		free(expr_val.id);
	// 	break;
	// 	case fl::parser::TACNodeType::NUM:
	// 		std::println("type=numerical,  value={}", expr_val.num);
    // 		val.emplace<1>(expr_val.num);
	// 	break;
	// 	case fl::parser::TACNodeType::TAC_INDEX:
	// 		std::println("type=tac_ref,    value=[{}]", expr_val.tac_index);
    // 		val.emplace<2>(expr_val.tac_index);
	// 	break;
	// 	default:
    // 		panic("internal compiler error: expression 'L + R' - L type is undefined");
    // 	}
    // }

} // namespace fl::parser
