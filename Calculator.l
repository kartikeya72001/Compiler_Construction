%{
    #include<stdio.h>
    #include<math.h>
    int op = 0;
    float a,b;
%}

dig [0-9]+|([0-9]*)"."([0-9]+)
add "+"
sub "-"
div "/"
mul "*"
pow "^"
mod "%"
nl \n


%%

{dig} {
    if(op==0)
        a = atof(yytext);
    else{
        b = atof(yytext);
        switch(op){
            case 1: a = a + b;
                break;
            case 2: a = a - b;
                break;
            case 3: a = a/b;
                break;
            case 4: a = a*b;
                break;
            case 5: a = pow(a,b);
                break;
            case 6: a = a%b;
                break;
            default:  break;
        }
        op = 0;
    }
}
{add} {op = 1;}
{sub} {op = 2;}
{div} {op = 3;}
{mul} {op = 4;}
{pow} {op = 5;}
{mod} {op = 6;}
{nl} {printf("%f\n\n",a);}

%%
main(int argv, char* argc[]){
    yylex();   
}
yywrap(){
    return 1;
}