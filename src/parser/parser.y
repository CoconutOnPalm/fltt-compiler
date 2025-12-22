%code requires {
    #include <string>
}

%{
    #include <print>
    #include <string_view>
    
    extern int yylineno;

    int yylex();
    int yyerror(const std::string_view s);
%}


%union {
    int val;
    const char* word;
}


%token <val>    VAL
%token <word>   ID

%token <word>   

%left '='
%left '+' '-'
%left '*' '/' '%'

%%


S
    : %empty
;


%%


int yyerror(const std::string_view s) 
{
    std::println("\033[31m[ERROR]\033[0m: {} in line {}", s, yylineno - 1);
    return 0;
}


int main()
{
    yyparse();
}