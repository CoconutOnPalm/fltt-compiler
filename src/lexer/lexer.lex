
%option noyywrap
%option yylineno


%{
#include <string>
#include <print>
#include <cstring>

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

comment     \#.*\n


%%

[ \t\n]+        { /* skip whitespaces */ }

{comment}       { /* skip comments */ }

ENDIF           { return ENDIF; }
IF              { return IF; }
THEN            { return THEN; }
ELSE            { return ELSE; }

ENDWHILE        { return ENDWHILE; }
DO              { return DO; }
WHILE           { return WHILE; }

ENDFOR          { return ENDFOR; }
FOR             { return FOR; }
FROM            { return FROM; }
DOWNTO          { return DOWNTO; }
TO              { return TO; }

REPEAT          { return REPEAT; }
UNTIL           { return UNTIL; }

READ            { return READ; }
WRITE           { return WRITE; }

PROGRAM         { return PROGRAM; }
PROCEDURE       { return PROCEDURE; }
IS              { return IS; }
IN              { return IN; }
END             { return END; }

T               { return T; }
I               { return I; }
O               { return O; }

{identifier}    { yylval.id = strdup(yytext); return pidentifier; }

{number}        { yylval.num = std::stoull(yytext); return num; }

"+"              { return ADD;  }
"-"              { return SUB;  }
"*"              { return MULT; }
"/"              { return DIV;  }
"%"              { return MOD;  }

":="             { return ASSIGN; }
"!="             { return NEQ; }
">"              { return GT;  }
"<"              { return LT;  }
">="             { return GEQ; }
"<="             { return LEQ; }
"="              { return EQ;  }


","              { return ','; }
";"              { return ';'; }

.                { return static_cast<int>(yytext[0]); }

%%