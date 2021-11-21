%{
#include<stdio.h>
#include<string.h>
%}
%%
#.* {
    printf("Preprocessor Directive: %s\n",yytext);
}
"while"|"if"|"else"|"string"|"int"|"double"|"char"|"for"|"break"|"continue"|"do"|"return"|"void"|"switch"|"typedef"|"struct"|"goto"|"float"|"printf"|"scanf" {
    printf("Keyword : %s\n", yytext);
}
"<="|"=="|"="|"++"|"-"|"*"|"+"|"--" {
    printf("Operator : %s\n", yytext);
}
[0-9]*"."[0-9]+|[0-9]* {
    printf("Constant : %s\n", yytext);
}
[a-zA-Z_][a-zA-Z0-9_]* {
    printf("Identifier : %s\n", yytext);
}
[(){}|,;]    {
    printf("Seperator : %s\n", yytext);
}
.
%%

int yywrap(void){}

int main(){
	yylex();
	return 0;
}