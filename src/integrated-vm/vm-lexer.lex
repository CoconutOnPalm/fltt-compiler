/*
 * Lexer interpretera maszyny wirtualnej do projektu z JFTT2025
 *
 * Autor: Maciek Gębala
 * http://ki.pwr.edu.pl/gebala/
 * 2025-11-15
 * 
 * Modified by Adam Kostrzewski
*/

%option noyywrap
%option yylineno
%option nounput

%{
    #include "vm-parser.hpp"
    #include "instructions.hpp"

    int vm_yylex();
%}

%%


\#.*\n		    ;
[ \t]+          ;
READ            { vm_yylval = fl::vm::READ;   return COM_0; };
WRITE           { vm_yylval = fl::vm::WRITE;  return COM_0; };
LOAD            { vm_yylval = fl::vm::LOAD;   return COM_1; };
STORE           { vm_yylval = fl::vm::STORE;  return COM_1; };
RLOAD           { vm_yylval = fl::vm::RLOAD;  return COM_2; };
RSTORE          { vm_yylval = fl::vm::RSTORE; return COM_2; };
ADD             { vm_yylval = fl::vm::ADD;    return COM_2; };
SUB             { vm_yylval = fl::vm::SUB;    return COM_2; };
SWP             { vm_yylval = fl::vm::SWP;    return COM_2; };
RST             { vm_yylval = fl::vm::RST;    return COM_2; };
INC             { vm_yylval = fl::vm::INC;    return COM_2; };
DEC             { vm_yylval = fl::vm::DEC;    return COM_2; };
SHL             { vm_yylval = fl::vm::SHL;    return COM_2; };
SHR             { vm_yylval = fl::vm::SHR;    return COM_2; };
JUMP            { vm_yylval = fl::vm::JUMP;   return JUMP_1; };
JPOS            { vm_yylval = fl::vm::JPOS;   return JUMP_1; };
JZERO           { vm_yylval = fl::vm::JZERO;  return JUMP_1; };
CALL		    { vm_yylval = fl::vm::CALL;   return JUMP_1; };
RTRN		    { vm_yylval = fl::vm::RTRN;   return JUMP_0; };
HALT            { vm_yylval = fl::vm::HALT;   return STOP; };
a               { vm_yylval = 0;      return REG; };
b               { vm_yylval = 1;      return REG; };
c               { vm_yylval = 2;      return REG; };
d               { vm_yylval = 3;      return REG; };
e               { vm_yylval = 4;      return REG; };
f               { vm_yylval = 5;      return REG; };
g               { vm_yylval = 6;      return REG; };
h               { vm_yylval = 7;      return REG; };
[0-9]+		    { vm_yylval = std::atoll(vm_yytext);  return NUMBER; };
\n              ;
.               return ERROR;

%%
