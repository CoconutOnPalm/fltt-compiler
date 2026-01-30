%code requires {
    #include "../compiler.hpp"
    #include "parser_utils.hpp"
}

%code provides {
    // extern fl::Compiler compiler;
}

%code {
    // fl::Compiler compiler;
}

%{
    #include <print>
    #include <string_view>
    #include <variant>
    #include "../compiler.hpp"
    
    extern int yylineno;

    int yylex();
    int yyerror(fl::Compiler& compiler, const char* s);
    void yyset_in(FILE* in_str);
%}


%parse-param { fl::Compiler& compiler }

%define parse.error verbose
%define parse.trace


%union {
    uint64_t num;
    char* id;
    
    fl::ASTNode* ast;
    fl::ast::Block* block;
    fl::ast::Condition* cond;
    fl::SymbolTable* st;
    fl::ast::Identifier* ident;
    fl::ast::ArgsDecl* argdecl;
    fl::ast::ProcDecl* procdecl;
    fl::ast::Params* param;
    fl::ast::ProcCall* proccall;
}



%token <num>        num
%token <id>         pidentifier

%type <block>       block
%type <ident>       identifier
%type <ast>         expression
%type <cond>        condition
%type <st>          declarations
%type <param>       args
%type <ast>         procedure_call
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
        std::println("parsing complete");
    }
;

procedures 
    : procedures PROCEDURE procedure_head IS declarations IN block END
    {
        fl::ast::ProcDecl* head = $<procdecl>3;
        fl::SymbolTable* symbol_table = $<st>5;
        fl::ast::Block* block = $<block>7;
        
        compiler.defineProcedure(head->name(), head, symbol_table, block);

        // delete head;
        // delete symbol_table;
        // delete block;
    }
    | procedures PROCEDURE procedure_head IS IN block END
    {
        fl::ast::ProcDecl* head = $<procdecl>3;
        fl::SymbolTable* symbol_table = nullptr;
        fl::ast::Block* block = $<block>6;

        compiler.defineProcedure(head->name(), head, symbol_table, block);

        // delete head;
        // // delete symbol_table; <-- nullptr
        // delete block;
    }
    | %empty
;

main 
    : PROGRAM IS declarations IN block END
    {
        fl::SymbolTable* symbol_table = $<st>3;   
        fl::ast::Block* block = $<block>5;
        
        compiler.defineMain(symbol_table, block);

        // delete symbol_table;
        // delete block;
    }
    | PROGRAM IS IN block END
    {
        fl::SymbolTable* symbol_table = nullptr;   
        fl::ast::Block* block = $<block>4;

        compiler.defineMain(symbol_table, block);

        // // delete symbol_table; <-- nullptr
        // delete block;
    }
;

block 
    : block statement
    {
        fl::ast::Block* this_block = $<block>1;
        fl::ASTNode* statement = $<ast>2;
        this_block->addStatement(statement);
        $$ = this_block;

    }
    | statement
    {
        fl::ast::Block* new_block = new fl::ast::Block;
        fl::ASTNode* statement = $<ast>1;
        new_block->addStatement(statement);
        $$ = new_block;
    }
;

statement 
    : identifier ASSIGN expression ';'
    {
        fl::ast::Identifier* original = $<ident>1;
        fl::ASTNode* rvalue = $<ast>3;

        if (dynamic_cast<fl::ast::IndexOf*>(original))
        {
            $<ast>$ = new fl::ast::Assign(original, rvalue);
        }
        else
        {
            fl::ast::LvalIdentifier* lvalue = new fl::ast::LvalIdentifier(original->identifier); 
            $<ast>$ = new fl::ast::Assign(lvalue, rvalue);
        }
    }
    | IF condition THEN block[I] ELSE block[E] ENDIF
    {
        $<ast>$ = new fl::ast::IfElse($<cond>2, $<block>I, $<block>E);
    }
    | IF condition THEN block ENDIF
    {
        $<ast>$ = new fl::ast::If($<cond>2, $<block>4);
    }
    | WHILE condition DO block ENDWHILE
    {
        $<ast>$ = new fl::ast::While($<cond>2, $<block>4);
    }
    | REPEAT block UNTIL condition ';'
    {
        $<ast>$ = new fl::ast::DoWhile($<cond>4, $<block>2);
    }
    | FOR pidentifier FROM value TO value DO block ENDFOR
    {
        $<ast>$ = new fl::ast::For(new fl::ast::Identifier($<id>2), $<ast>4, $<ast>6, $<block>8, 1);
    }
    | FOR pidentifier FROM value DOWNTO value DO block ENDFOR
    {
        $<ast>$ = new fl::ast::For(new fl::ast::Identifier($<id>2), $<ast>4, $<ast>6, $<block>8, -1);
    }
    | procedure_call ';'
    {
        $<ast>$ = $<ast>1;
    }
    | READ identifier ';'
    {
        fl::ast::Identifier* original = $<ident>2;

        if (dynamic_cast<fl::ast::IndexOf*>(original))
        {
            $<ast>$ = new fl::ast::Read(original);
        }
        else
        {
            fl::ast::LvalIdentifier* lvalue = new fl::ast::LvalIdentifier(original->identifier); 
            $<ast>$ = new fl::ast::Read(lvalue);
        }
    }
    | WRITE value ';'
    {
        $<ast>$ = new fl::ast::Write($<ast>2);
    }
;

procedure_head 
    : pidentifier '(' args_decl ')'
    {
        fl::ast::ArgsDecl* args = $<argdecl>3;
        fl::ast::ProcDecl* procedure = new fl::ast::ProcDecl($<id>1, std::move(*args));

        $<procdecl>$ = procedure;

        delete args;
        // free($<id>1);
    }
;

procedure_call 
    : pidentifier '(' args ')'
    {
        fl::ast::Params* params = $<param>3;
        fl::ASTNode* proc_call = new fl::ast::ProcCall($<id>1, std::move(*params));
        // free(params);
        // free($<id>1);

        $<ast>$ = proc_call;
    }
;

declarations 
    : declarations ',' pidentifier
    {
        fl::SymbolTable* symbol_table = $<st>1;
        symbol_table->add<fl::Variable>($<id>3);
        $<st>$ = symbol_table;
        // free($<id>3);
    }
    | declarations ',' pidentifier '[' num ':' num ']'
    {
        fl::SymbolTable* symbol_table = $<st>1;
        symbol_table->add<fl::Array>($<id>3, $<num>5, $<num>7);
        $<st>$ = symbol_table;
        // free($<id>3);
    }
    | pidentifier
    {
        fl::SymbolTable* symbol_table = new fl::SymbolTable;
        symbol_table->add<fl::Variable>($<id>1);
        $<st>$ = symbol_table;
        // free($<id>1);
    }
    | pidentifier '[' num ':' num ']'
    {
        fl::SymbolTable* symbol_table = new fl::SymbolTable;
        symbol_table->add<fl::Array>($<id>1, $<num>3, $<num>5);
        $<st>$ = symbol_table;
        // free($<id>1);
    }
;

args_decl 
    : args_decl ',' type pidentifier
    {
        fl::ast::ArgsDecl* args = $<argdecl>1;
        args->add(fl::Argument($<id>4, static_cast<fl::ArgType>($<num>3)));
        $<argdecl>$ = args;
    }
    | type pidentifier
    {
        fl::ast::ArgsDecl* args = new fl::ast::ArgsDecl;
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
        fl::ast::Params* params = $<param>1;
        params->add($<id>3);
        // free($<id>3);
        $<param>$ = params;
    }
    | pidentifier
    {
        fl::ast::Params* params = new fl::ast::Params;
        params->add($<id>1);
        // free($<id>1);
        $<param>$ = params;
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
        $<ast>$ = new fl::ast::Addition(left, right);
    }
    | value[L] SUB value[R]
    {
        $<ast>$ = new fl::ast::Substraction($<ast>L, $<ast>R);
    }
    | value[L] MULT value[R]
    {
        $<ast>$ = new fl::ast::Multiplication($<ast>L, $<ast>R);
    }
    | value[L] DIV value[R]
    {
        $<ast>$ = new fl::ast::Division($<ast>L, $<ast>R);
    }
    | value[L] MOD value[R]
    {
        $<ast>$ = new fl::ast::Modulo($<ast>L, $<ast>R);
    }
;

condition 
    : value[L] EQ value[R]
    {
        $<cond>$ = new fl::ast::Condition(fl::CondOp::EQ, $<ast>L, $<ast>R);
    }
    | value[L] NEQ value[R]
    {
        $<cond>$ = new fl::ast::Condition(fl::CondOp::NEQ, $<ast>L, $<ast>R);
    }
    | value[L] GT value[R]
    {
        $<cond>$ = new fl::ast::Condition(fl::CondOp::GT, $<ast>L, $<ast>R);
    }
    | value[L] LT value[R]
    {
        $<cond>$ = new fl::ast::Condition(fl::CondOp::LT, $<ast>L, $<ast>R);
    }
    | value[L] GEQ value[R]
    {
        $<cond>$ = new fl::ast::Condition(fl::CondOp::GEQ, $<ast>L, $<ast>R);
    }
    | value[L] LEQ value[R]
    {
        $<cond>$ = new fl::ast::Condition(fl::CondOp::LEQ, $<ast>L, $<ast>R);
    }
;

value 
    : num
    {
        $<ast>$ = new fl::ast::Number($<num>1);
    }
    | identifier
    {
        $<ast>$ = $<ident>1;
    }
;

identifier 
    : pidentifier
    {
        $<ident>$ = new fl::ast::Identifier($<id>1);
        // free($<id>1);
    }
    | pidentifier '[' pidentifier ']'
    {
        fl::ASTNode* ind = new fl::ast::Identifier($<id>3);
        fl::ast::Identifier* node = new fl::ast::IndexOf($<id>1, ind);

        $<ident>$ = node;

        // free($<id>1);
        // free($<id>3);
    }
    | pidentifier '[' num ']'
    {
        fl::ASTNode* ind = new fl::ast::Number($<num>3);
        fl::ast::Identifier* node = new fl::ast::IndexOf($<id>1, ind);

        $<ident>$ = node;

        // free($<id>1);
    }
;

%%


int yyerror(fl::Compiler& compiler, const char* s) 
{
    std::string_view msg { s };
    std::println("\033[31m[ERROR]\033[0m: {} at line {}", msg, yylineno);
    return 0;
}


void run_parser(fl::Compiler& compiler, FILE* in_str)
{
    yyset_in(in_str);
    yyparse(compiler);
}