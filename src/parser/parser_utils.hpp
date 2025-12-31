#pragma once

#include <variant>

#include "../AST/syntax_tree.hpp"
#include "../AST/blocks/block.hpp"
#include "../AST/blocks/expression.hpp"
#include "../AST/blocks/if.hpp"
#include "../AST/blocks/ifelse.hpp"
#include "../AST/blocks/while.hpp"
#include "../AST/blocks/dowhile.hpp"


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
