%code requires {
    #include "../compiler.hpp"
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
    
    extern int yylineno;

    int yylex();
    int yyerror(const char* s);
    void yyset_in(FILE* in_str);
%}


%union {
    int val;
    char* identifier;
}


%token <val>            num
%token <identifier>     pidentifier

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
        compiler.pushProcedure($<identifier>3);
    }
    | procedures PROCEDURE procedure_head IS IN statements END
    {
        compiler.pushProcedure($<identifier>3);
    }
    | %empty
;

main 
    : PROGRAM IS declarations IN statements END
    {
        compiler.pushProcedure("__prog_start");
    }
    | PROGRAM IS IN statements END
    {
        compiler.pushProcedure("__prog_start");
    }
;

statements 
    : statements statement
    | statement
;

statement 
    : identifier ASSIGN expression ';'
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
        $<identifier>$ = $<identifier>1;
    }
;

procedure_call 
    : pidentifier '(' args ')'
;

declarations 
    : declarations ',' pidentifier
    {
        compiler.addSymbol<fl::Variable>($<identifier>3);
        free($<identifier>3);
    }
    | declarations ',' pidentifier '[' num ':' num ']'
    {
        compiler.addSymbol<fl::Array>($<identifier>3, $<val>5, $<val>7);
        free($<identifier>3);
    }
    | pidentifier
    {
        compiler.addSymbol<fl::Variable>($<identifier>1);
        free($<identifier>1);
    }
    | pidentifier '[' num ':' num ']'
    {
        compiler.addSymbol<fl::Array>($<identifier>1, $<val>3, $<val>5);
        free($<identifier>1);
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
    | value[L] ADD  value[R]
    | value[L] SUB  value[R]
    | value[L] MULT value[R]
    | value[L] DIV  value[R]
    | value[L] MOD  value[R]
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
    | identifier
;

identifier 
    : pidentifier
    | pidentifier '[' pidentifier ']'
    | pidentifier '[' num ']'
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