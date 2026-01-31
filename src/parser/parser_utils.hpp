#pragma once

#include <variant>

#include "../utils/const.hpp"

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

} // namespace fl::parser
