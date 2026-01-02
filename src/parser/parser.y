%code requires {
    #include "../compiler.hpp"
    #include "parser_utils.hpp"
}

%code provides {
    extern fl::Compiler compiler;
}

%code {
    fl::Compiler compiler;
}

%{
    #include <print>
    #include <string_view>
    #include <variant>
    
    extern int yylineno;

    int yylex();
    int yyerror(const char* s);
    void yyset_in(FILE* in_str);
%}


%union {
    uint64_t num;
    char* id;
    // struct {
    //     size_t tac_index;
    //     uint64_t num;
    //     char* id;
    //     int type;
    // } val_t;
    fl::ASTNode* ast;
    fl::Block* block;
    fl::Condition* cond;
    fl::SymbolTable* st;
    fl::PTNode* pt;
    fl::ArgsDecl* argdecl;
    fl::ProcDecl* procdecl;
}



%token <num>        num
%token <id>         pidentifier

%type <block>       block
%type <ast>         identifier
%type <ast>         expression
%type <cond>        condition
%type <st>          declarations
%type <pt>          procedure_call
%type <argdecl>     args_decl
%type <procdecl>    procedure_head


/* KEYWORDS */
%token ENDIF
%token IF
%token THEN
%token ELSE
%token ENDWHILE
%token DO
%token WHILE
%token ENDFOR
%token FOR
%token FROM
%token DOWNTO
%token TO
%token REPEAT
%token UNTIL
%token READ
%token WRITE
%token PROGRAM
%token PROCEDURE
%token IS
%token IN
%token END
%token T
%token I
%token O


/* OPERATORS */
%token ADD
%token SUB
%token MULT
%token DIV
%token MOD

%token ASSIGN
%token EQ
%token NEQ
%token GT
%token LT
%token GEQ
%token LEQ



/* gramar does not need precedence */
/* %left ASSIGN 
%left EQ NEQ
%left GT LT GEQ LEQ
%left ADD SUB
%left MULT DIV MOD */


/* %type program_all
%type procedures
%type main
%type procedure_head
%type procedure_call
%type declarations
%type block
%type statement
%type args_decl
%type args */

%%


program_all 
    : procedures main
    {
        compiler.__debug_print();
    }
;

procedures 
    : procedures PROCEDURE procedure_head IS declarations IN block END
    {
        fl::ProcDecl* head = $<procdecl>3;
        head->generateTAC();
        fl::SymbolTable* symbol_table = $<st>5;
        symbol_table->__debug_print();
        fl::Block* block = $<block>7;
        block->generateTAC();
    }
    | procedures PROCEDURE procedure_head IS IN block END
    {
        fl::ProcDecl* head = $<procdecl>3;
        head->generateTAC();
        fl::Block* block = $<block>6;
        block->generateTAC();
    }
    | %empty
;

main 
    : PROGRAM IS declarations IN block END
    {
        fl::SymbolTable* symbol_table = $<st>3;   
        fl::Block* block = $<block>5;
    }
    | PROGRAM IS IN block END
    {
        fl::Block* block = $<block>4;
        block->generateTAC();
    }
;

block 
    : block statement
    {
        fl::Block* this_block = $<block>1;
        fl::ASTNode* statement = $<ast>2;
        this_block->addStatement(statement);
        $$ = this_block;
        // block_buffer->addStatement(statement);

    }
    | statement
    {
        // block_buffer = std::make_unique<fl::Block>();
        fl::Block* new_block = new fl::Block;
        fl::ASTNode* statement = $<ast>1;
        new_block->addStatement(statement);
        $$ = new_block;
        // block_buffer->addStatement(statement);
        
    }
;

statement 
    : identifier ASSIGN expression ';'
    {
        fl::ASTNode* lvalue = $<ast>1;
        fl::ASTNode* rvalue = $<ast>3;
        $<ast>$ = new fl::Expression(fl::Operator::ASSIGN, lvalue, rvalue);

        // auto lvalue = $<val_t>1;
        // auto rvalue = $<val_t>3;

        // fl::tacval_t left, right;
        // fl::parser::assignValueVariant(left, lvalue);
        // fl::parser::assignValueVariant(right, rvalue);

        // // size_t tac_index = compiler.pushExpression(fl::Operator::ASSIGN, left, right);
        // size_t tac_index = compiler.pushStatement<fl::Expression>(fl::Operator::ASSIGN, left, right);
    }
    | IF condition THEN block[I] ELSE block[E] ENDIF
    {
        $<ast>$ = new fl::IfElse($<cond>2, $<block>I, $<block>E);
    }
    | IF condition THEN block ENDIF
    {
        $<ast>$ = new fl::If($<cond>2, $<block>4);
    }
    | WHILE condition DO block ENDWHILE
    {
        $<ast>$ = new fl::While($<cond>2, $<block>4);
    }
    | REPEAT block UNTIL condition ';'
    {
        $<ast>$ = new fl::DoWhile($<cond>2, $<block>4);
    }
    | FOR pidentifier FROM value TO value DO block ENDFOR
    {
        $<ast>$ = new fl::For(new fl::Identifier($<id>2), $<ast>4, $<ast>6, $<block>8, 1);
    }
    | FOR pidentifier FROM value DOWNTO value DO block ENDFOR
    {
        $<ast>$ = new fl::For(new fl::Identifier($<id>2), $<ast>4, $<ast>6, $<block>8, -1);
    }
    | procedure_call ';'
    {

    }
    | READ identifier ';'
    | WRITE value ';'
;

procedure_head 
    : pidentifier '(' args_decl ')'
    {
        fl::ArgsDecl* args = $<argdecl>3;
        fl::ProcDecl* procedure = new fl::ProcDecl($<id>1, std::move(*args));

        $<procdecl>$ = procedure;

        delete args;
        free($<id>1);
    }
;

procedure_call 
    : pidentifier '(' args ')'
    {
        fl::PTNode* proc = new fl::PTNode($<id>1);
        $<pt>$ = proc;
        free($<id>1);
    }
;

declarations 
    : declarations ',' pidentifier
    {
        fl::SymbolTable* symbol_table = $<st>1;
        symbol_table->add<fl::Variable>($<id>3);
        $<st>$ = symbol_table;
        free($<id>3);
    }
    | declarations ',' pidentifier '[' num ':' num ']'
    {
        fl::SymbolTable* symbol_table = $<st>1;
        symbol_table->add<fl::Array>($<id>3, $<num>5, $<num>7);
        $<st>$ = symbol_table;
        // compiler.addSymbol<fl::Array>($<id>3, $<num>5, $<num>7);
        free($<id>3);
    }
    | pidentifier
    {
        fl::SymbolTable* symbol_table = new fl::SymbolTable;
        symbol_table->add<fl::Variable>($<id>1);
        $<st>$ = symbol_table;
        // compiler.addSymbol<fl::Variable>($<id>1);
        free($<id>1);
    }
    | pidentifier '[' num ':' num ']'
    {
        fl::SymbolTable* symbol_table = new fl::SymbolTable;
        symbol_table->add<fl::Array>($<id>1, $<num>3, $<num>5);
        $<st>$ = symbol_table;
        // compiler.addSymbol<fl::Array>($<id>1, $<num>3, $<num>5);
        free($<id>1);
    }
;

args_decl 
    : args_decl ',' type pidentifier
    {
        fl::ArgsDecl* args = $<argdecl>1;
        args->add(fl::Argument($<id>4, static_cast<fl::ArgType>($<num>3)));
        $<argdecl>$ = args;
    }
    | type pidentifier
    {
        fl::ArgsDecl* args = new fl::ArgsDecl;
        args->add(fl::Argument($<id>2, static_cast<fl::ArgType>($<num>1)));
        $<argdecl>$ = args;
    }
;

type 
    : T         { $<num>$ = static_cast<int>(fl::ArgType::ARRAY); }
    | I         { $<num>$ = static_cast<int>(fl::ArgType::IN);    }
    | O         { $<num>$ = static_cast<int>(fl::ArgType::OUT);   }
    | %empty    { $<num>$ = static_cast<int>(fl::ArgType::NONE);  }
;

args 
    : args ',' pidentifier
    {

    }
    | pidentifier
    {

    }
;

expression 
    : value
    {
        $<ast>$ = $<ast>1;
    }
    | value[L] ADD value[R]
    {
        fl::ASTNode* left = $<ast>L;
        fl::ASTNode* right = $<ast>R;
        $<ast>$ = new fl::Expression(fl::Operator::ADD, left, right);
        // auto left_val = $<val_t>L;
        // auto right_val = $<val_t>R;

        // fl::tacval_t left, right;
        // fl::parser::assignValueVariant(left, left_val);
        // fl::parser::assignValueVariant(right, right_val);

        // // size_t tac_index = compiler.pushExpression(fl::Operator::ADD, left, right);
        // size_t tac_index = compiler.pushStatement<fl::Expression>(fl::Operator::ADD, left, right);

        // $<val_t>$.type = fl::parser::TACNodeType::TAC_INDEX;
        // $<val_t>$.tac_index = tac_index;
    }
    | value[L] SUB value[R]
    {
        $<ast>$ = new fl::Expression(fl::Operator::SUB, $<ast>L, $<ast>R);
    }
    | value[L] MULT value[R]
    {
        $<ast>$ = new fl::Expression(fl::Operator::MULT, $<ast>L, $<ast>R);
    }
    | value[L] DIV value[R]
    {
        $<ast>$ = new fl::Expression(fl::Operator::DIV, $<ast>L, $<ast>R);
    }
    | value[L] MOD value[R]
    {
        $<ast>$ = new fl::Expression(fl::Operator::MOD, $<ast>L, $<ast>R);
    }
;

condition 
    : value[L] EQ  value[R]
    {
        $<cond>$ = new fl::Condition(fl::CondOp::EQ, $<ast>L, $<ast>R);
    }
    | value[L] NEQ value[R]
    {
        $<cond>$ = new fl::Condition(fl::CondOp::NEQ, $<ast>L, $<ast>R);
    }
    | value[L] GT  value[R]
    {
        $<cond>$ = new fl::Condition(fl::CondOp::GT, $<ast>L, $<ast>R);

        // auto left_val = $<val_t>L;
        // auto right_val = $<val_t>R;

        // fl::tacval_t left, right;
        // fl::parser::assignValueVariant(left, left_val);
        // fl::parser::assignValueVariant(right, right_val);

        // size_t tac_index = compiler.pushStatement<fl::Condition>(fl::CondOp::GT, left, right);

        // $<val_t>$.type = fl::parser::TACNodeType::TAC_INDEX;
        // $<val_t>$.tac_index = tac_index;
    }
    | value[L] LT  value[R]
    {
        $<cond>$ = new fl::Condition(fl::CondOp::LT, $<ast>L, $<ast>R);
    }
    | value[L] GEQ value[R]
    {
        $<cond>$ = new fl::Condition(fl::CondOp::GEQ, $<ast>L, $<ast>R);
    }
    | value[L] LEQ value[R]
    {
        $<cond>$ = new fl::Condition(fl::CondOp::LEQ, $<ast>L, $<ast>R);
    }
;

value 
    : num
    {
        $<ast>$ = new fl::Number($<num>1);
        // $<val_t>$.type = fl::parser::TACNodeType::NUM;
        // $<val_t>$.num = $<num>1;
    }
    | identifier
    {
        $<ast>$ = $<ast>1;
        // $<val_t>$ = $<val_t>1;
    }
;

identifier 
    : pidentifier
    {
        $<ast>$ = new fl::Identifier($<id>1);
        free($<id>1);
        // $<val_t>$.type = fl::parser::TACNodeType::IDENTIFIER;
        // $<val_t>$.id = $<id>1;
    }
    | pidentifier '[' pidentifier ']'
    {
        fl::ASTNode* arr = new fl::Identifier($<id>1);
        fl::ASTNode* ind = new fl::Identifier($<id>3);
        fl::ASTNode* node = new fl::Expression(fl::Operator::INDEX, arr, ind);

        $<ast>$ = node;

        free($<id>1);
        free($<id>3);
        
        // fl::tacval_t arr; arr.emplace<0>(std::string_view{$<id>1});
        // fl::tacval_t ind; ind.emplace<0>(std::string_view{$<id>3});
        // // size_t tac_index = compiler.pushExpression(fl::Operator::INDEX, arr, ind);
        // size_t tac_index = compiler.pushStatement<fl::Expression>(fl::Operator::INDEX, arr, ind);
        // $<val_t>$.type = fl::parser::TACNodeType::TAC_INDEX;
        // $<val_t>$.tac_index = tac_index;
        // free($<id>1);
        // free($<id>3);
    }
    | pidentifier '[' num ']'
    {
        fl::ASTNode* arr = new fl::Identifier($<id>1);
        fl::ASTNode* ind = new fl::Number($<num>3);
        fl::ASTNode* node = new fl::Expression(fl::Operator::INDEX, arr, ind);

        $<ast>$ = node;

        free($<id>1);
        
        // fl::tacval_t arr; arr.emplace<0>(std::string_view{$<id>1});
        // fl::tacval_t ind; ind.emplace<1>($<num>3);
        // // size_t tac_index = compiler.pushExpression(fl::Operator::INDEX, arr, ind);
        // size_t tac_index = compiler.pushStatement<fl::Expression>(fl::Operator::INDEX, arr, ind);
        // $<val_t>$.type = fl::parser::TACNodeType::TAC_INDEX;
        // $<val_t>$.tac_index = tac_index;
        // free($<id>1);
    }
;

%%


int yyerror(const char* s) 
{
    std::string_view msg { s };
    std::println("\033[31m[ERROR]\033[0m: {} at line {}", msg, yylineno);
    return 0;
}


void run_parser(FILE* in_str)
{
    yyset_in(in_str);
    yyparse();
}