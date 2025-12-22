
%option noyywrap
%option yylineno


%{
#include <string>
#include <print>

#include "../parser/parser.hpp"

int yylex();
%}


number      [0-9]+
identifier  [_a-z]+

/* 
keywords:
    PROGRAM
    PROCEDURE IS IN END
    IF THEN ELSE ENDIF
    WHILE DO ENDWHILE
    FOR FROM TO DOWNTO DO ENDFOR
    REPEAT UNTIL
    READ
    WRITE
    T I O 
*/
keyword     [A-Z]+


comment     \#.*\n


%%

[ \t\n]+        { /* skip whitespaces */ }

{comment}       { /* skip comments */ }

{keyword}       { return yytext; }
{identifier}    { yylval.identifier = yytext; return ID; }

{number}        { yylval.val = std::stoull(yytext); return VAL; }

[\+\-\*\/\%]    { return yytext; }

%%