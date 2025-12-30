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
    #include "../tac/triple.hpp"
    
    extern int yylineno;

    int yylex();
    int yyerror(const char* s);
    void yyset_in(FILE* in_str);
%}


%union {
    uint64_t num;
    char* id;
    struct {
        size_t tac_index;
        uint64_t num;
        char* id;
        int type;
    } val_t;
}



%token <num>        num
%token <id>         pidentifier
%type  <val_t>      identifier
%type  <val_t>      expression


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
%type statements
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
    : procedures PROCEDURE procedure_head IS declarations IN statements END
    {
        compiler.pushProcedure($<id>3);
    }
    | procedures PROCEDURE procedure_head IS IN statements END
    {
        compiler.pushProcedure($<id>3);
    }
    | %empty
;

main 
    : PROGRAM IS declarations IN statements END
    {
        compiler.pushProcedure("__PROG_START");
    }
    | PROGRAM IS IN statements END
    {
        compiler.pushProcedure("__PROG_START");
    }
;

statements 
    : statements statement
    | statement
;

statement 
    : identifier ASSIGN expression ';'
    {
        auto lvalue = $<val_t>1;
        auto rvalue = $<val_t>3;

        fl::tacval_t left, right;
        fl::parser::assignValueVariant(left, lvalue);
        fl::parser::assignValueVariant(right, rvalue);

        size_t tac_index = compiler.pushExpression(fl::Operator::ASSIGN, left, right).index;
    }
    | IF condition THEN statements ELSE statements ENDIF
    | IF condition THEN statements ENDIF
    | WHILE condition DO statements ENDWHILE
    | REPEAT statements UNTIL condition ';'
    | FOR pidentifier FROM value TO value DO statements ENDFOR
    | FOR pidentifier FROM value DOWNTO value DO statements ENDFOR
    | procedure_call ';'
    | READ identifier ';'
    | WRITE value ';'
;

procedure_head 
    : pidentifier '(' args_decl ')'
    {
        $<id>$ = $<id>1;
    }
;

procedure_call 
    : pidentifier '(' args ')'
;

declarations 
    : declarations ',' pidentifier
    {
        compiler.addSymbol<fl::Variable>($<id>3);
        free($<id>3);
    }
    | declarations ',' pidentifier '[' num ':' num ']'
    {
        compiler.addSymbol<fl::Array>($<id>3, $<num>5, $<num>7);
        free($<id>3);
    }
    | pidentifier
    {
        compiler.addSymbol<fl::Variable>($<id>1);
        free($<id>1);
    }
    | pidentifier '[' num ':' num ']'
    {
        compiler.addSymbol<fl::Array>($<id>1, $<num>3, $<num>5);
        free($<id>1);
    }
;

args_decl 
    : args_decl ',' type pidentifier
    | type pidentifier
;

type 
    : T 
    | I 
    | O 
    | %empty
;

args 
    : args ',' pidentifier
    | pidentifier
;

expression 
    : value
    {
        $<val_t>$ = $<val_t>1;
    }
    | value[L] ADD value[R]
    {
        auto left_val = $<val_t>L;
        auto right_val = $<val_t>R;

        fl::tacval_t left, right;
        fl::parser::assignValueVariant(left, left_val);
        fl::parser::assignValueVariant(right, right_val);

        size_t tac_index = compiler.pushExpression(fl::Operator::ADD, left, right).index;

        $<val_t>$.type = fl::parser::TACNodeType::TAC_INDEX;
        $<val_t>$.tac_index = tac_index;
    }
    | value[L] SUB value[R]
    {

    }
    | value[L] MULT value[R]
    {
        
    }
    | value[L] DIV value[R]
    {
        
    }
    | value[L] MOD value[R]
    {
        
    }
;

condition 
    : value[L] EQ  value[R]
    | value[L] NEQ value[R]
    | value[L] GT  value[R]
    | value[L] LT  value[R]
    | value[L] GEQ value[R]
    | value[L] LEQ value[R]
;

value 
    : num
    {
        $<val_t>$.type = fl::parser::TACNodeType::NUM;
        $<val_t>$.num = $<num>1;
    }
    | identifier
    {
        $<val_t>$ = $<val_t>1;
    }
;

identifier 
    : pidentifier
    {
        $<val_t>$.type = fl::parser::TACNodeType::IDENTIFIER;
        $<val_t>$.id = $<id>1;
    }
    | pidentifier '[' pidentifier ']'
    {
        fl::tacval_t arr; arr.emplace<0>(std::string_view{$<id>1});
        fl::tacval_t ind; ind.emplace<0>(std::string_view{$<id>3});
        size_t tac_index = compiler.pushExpression(fl::Operator::INDEX, arr, ind).index;
        $<val_t>$.type = fl::parser::TACNodeType::TAC_INDEX;
        $<val_t>$.tac_index = tac_index;
        free($<id>1);
        free($<id>3);
    }
    | pidentifier '[' num ']'
    {
        fl::tacval_t arr; arr.emplace<0>(std::string_view{$<id>1});
        fl::tacval_t ind; ind.emplace<1>($<num>3);
        size_t tac_index = compiler.pushExpression(fl::Operator::INDEX, arr, ind).index;
        $<val_t>$.type = fl::parser::TACNodeType::TAC_INDEX;
        $<val_t>$.tac_index = tac_index;
        free($<id>1);
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