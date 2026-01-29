/*
 * Parser interpretera maszyny wirtualnej do projektu z JFTT2025
 *
 * Autor: Maciek Gębala
 * http://ki.pwr.edu.pl/gebala/
 * 2025-11-15
 * 
 * Modified by Adam Kostrzewski
*/
%code requires { 
    #include <vector> 
    #include <utility>

    #define YYSTYPE long long
}

%{
    #include <iostream>
    #include <print>
    #include <utility>
    #include <vector>

    #include "instructions.hpp"
    #include "colors.hpp"

    #ifndef YY_TYPEDEF_YY_BUFFER_STATE
    #define YY_TYPEDEF_YY_BUFFER_STATE
    typedef struct yy_buffer_state *YY_BUFFER_STATE;
    #endif


    extern int yylineno;
    int vm_yylex( void );
    void vm_yyerror(std::vector<std::pair<int,long long>>& program, char const *s );
    YY_BUFFER_STATE vm_yy_scan_string(const char* str);
    void vm_yy_delete_buffer(YY_BUFFER_STATE buffer);

%}

%parse-param { std::vector<std::pair<int,long long>>& program }
%token COM_0
%token COM_1
%token COM_2
%token JUMP_0
%token JUMP_1
%token STOP
%token REG
%token NUMBER
%token ERROR
%%
input 
    : input line
    | %empty
;

line 
    : COM_0	        { program.push_back(std::make_pair($1,0));  }
    | COM_1 NUMBER  { program.push_back(std::make_pair($1,$2)); }
    | COM_2 REG	    { program.push_back(std::make_pair($1,$2)); }
    | JUMP_0        { program.push_back(std::make_pair($1,0));  }
    | JUMP_1 NUMBER { program.push_back(std::make_pair($1,$2)); }
    | STOP          { program.push_back(std::make_pair($1,0));  }
    | ERROR         { vm_yyerror(program, "Unrecognized symbol"); }
;

%%

void vm_yyerror(std::vector<std::pair<int,long long>>& program, char const *s)
{
    std::println(std::cerr, "[vm]: error line {}", yylineno);
    std::exit(-1);
}

void vm_run_parser(std::vector<std::pair<int,long long>>& program, const std::vector<std::string>& instructions) 
{
    // Convert vector to single string
    std::string input;
    for (const auto& line : instructions) {
        input += line + '\n';
    }
    
    // Create buffer from string using yy_scan_string
    YY_BUFFER_STATE buffer = vm_yy_scan_string(input.c_str());
    
    // Parse
    vm_yyparse(program);
    
    // Clean up buffer
    vm_yy_delete_buffer(buffer);
}