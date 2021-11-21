%{
    #include <stdio.h>
    #include <stdlib.h>
    int yyerror(char *msg);
    extern int yylex();
    int flag = 1;
%}

%token INT FLOAT CHAR DOUBLE VOID
%token FOR WHILE 
%token IF ELSE PRINTF 
%token STRUCT 
%token NUM ID
%token INCLUDE
%token DOT

%right '='
%left AND OR
%left '<' '>' LE GE EQ NE LT GT
%%

start:	Function 
	| Declaration
	;

/* Declaration block */
Declaration: Type Assignment ';' 
	| Assignment ';'  	
	| FunctionCall ';' 	
	| ArrayUsage ';'	
	| Type ArrayUsage ';'   
	| StructStmt ';'	
	| error	
	;

/* Assignment block */
Assignment: ID '=' Assignment
	| ID '=' FunctionCall
	| ID '=' ArrayUsage
	| ArrayUsage '=' Assignment
	| ID ',' Assignment
	| NUM ',' Assignment
	| ID '+' Assignment
	| ID '-' Assignment
	| ID '*' Assignment
	| ID '/' Assignment	
	| NUM '+' Assignment
	| NUM '-' Assignment
	| NUM '*' Assignment
	| NUM '/' Assignment
	| '\'' Assignment '\''	
	| '(' Assignment ')'
	| '-' '(' Assignment ')'
	| '-' NUM
	| '-' ID
	|   NUM
	|   ID
	;

/* Function Call Block */
FunctionCall : ID'('')'
	| ID'('Assignment')'
	;

/* Array Usage */
ArrayUsage : ID'['Assignment']'
	;

/* Function block */
Function: Type ID '(' ArgListOpt ')' CompoundStmt 
	;
ArgListOpt: ArgList
	|
	;
ArgList:  ArgList ',' Arg
	| Arg
	;
Arg:	Type ID
	;
CompoundStmt:	'{' StmtList '}'
	;
StmtList:	StmtList Stmt
	|
	;
Stmt:	WhileStmt
	| Declaration
	| ForStmt
	| IfStmt
	| PrintFunc
	| ';'
	;

/* Type Identifier block */
Type:	INT 
	| FLOAT
	| CHAR
	| DOUBLE
	| VOID 
	;

/* Loop Blocks */ 
WhileStmt: WHILE '(' Expr ')' Stmt  
	| WHILE '(' Expr ')' CompoundStmt 
	;

/* For Block */
ForStmt: FOR '(' Expr ';' Expr ';' Expr ')' Stmt 
       | FOR '(' Expr ';' Expr ';' Expr ')' CompoundStmt 
       | FOR '(' Expr ')' Stmt 
       | FOR '(' Expr ')' CompoundStmt 
	;

/* IfStmt Block */
IfStmt : IF '(' Expr ')' 
	 	Stmt 
	;

/* Struct Statement */
StructStmt : STRUCT ID '{' Type Assignment '}'  
	;

/* Print Function */
PrintFunc : PRINTF '(' Expr ')' ';'
	;

/*Expression Block*/
Expr:	
	| Expr LE Expr 
	| Expr GE Expr
	| Expr NE Expr
	| Expr EQ Expr
	| Expr GT Expr
	| Expr LT Expr
	| Assignment
	| ArrayUsage
	;
%%

int main(int argc, char *argv[]){
	yyparse();
    if(flag)
    	printf("Parsing Successful\n");
    return 0;
}
         
int yyerror(char *msg) {
    extern int yylineno;
	printf("Parsing Failed\nLine Number: %d %s\n",yylineno,msg);
	flag = 0;
	exit(1);
}