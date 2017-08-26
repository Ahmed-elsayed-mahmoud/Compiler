%{
#include <bits/stdc++.h>
#include <unistd.h>
#define TRUE 1
#define FALSE 0

using namespace std;

extern  int yylex();
extern  FILE *yyin;
void yyerror(const char * s);
extern int lineCounter;
ofstream fileOut("byteCode.j");	/* file for writing output */

typedef enum {INT_T, FLOAT_T, BOOL_T, VOID_T, ERROR_T} type_enum;

map<string,string> inst_list = {
	/* arithmetic operations */
	{"+", "add"},
	{"-", "sub"},
	{"/", "div"},
	{"*", "mul"},
	{"|", "or"},
	{"&", "and"},
	{"%", "rem"},

	/* relational operations */
	{"==", "if_icmpeq"},
	{"<=", "if_icmple"},
	{">=", "if_icmpge"},
	{"!=", "if_icmpne"},
	{">",  "if_icmpgt"},
	{"<",  "if_icmplt"}
};

map<string, pair<int,type_enum> > symbTab;
vector<string> codeList;
string outfileName ;

int varaiblesNum = 1; 	/* new variable will be issued this number, java starts with 1, 0 is 'this' */
int labelsCount = 0;	/* to generate labels */


void generateHeader(void);	/* generate  header for class to be able to compile the code*/
void generateFooter(void);	/* generate  footer for class to be able to compile the code*/
void arithCast(int from , int to, string op);
void cast (string x, int type_t1);
void genCode(vector<int> *list, int num);
void defineVar(string name, int type);
string getOperationName(string op);
string getLabel(int n);
string generateLabel();
bool checkId(string id);
vector<int> * merge (vector<int> *list1, vector<int>* list2);
void addLineCode(string x);
void printCode(void);
void printLineNumber(int num)
{
	addLineCode(".line "+ to_string(num));
}



%}

%code requires {
	#include <vector>
	using namespace std;
}

%start method_body

%union{
	int ival;     // int
	float fval;   // float
	int bval;     // boolean
	char * idval; // identifier
	char * aopval;// operation
	struct {      // synthesized attr
		int sType;
	} expr_type;
	struct {      // inherited attr
		vector<int> *trueList, *falseList;
	} bexpr_type;
	struct {
		vector<int> *nextList;
	} stmt_type;
	int sType;
}

%token <ival> INT
%token <fval> FLOAT
%token <bval> BOOL
%token <idval> IDENTIFIER
%token <aopval> ARITH_OP
%token <aopval> RELA_OP
%token <aopval> BOOL_OP

%token IF_WORD
%token ELSE_WORD
%token WHILE_WORD
%token FOR_WORD

%token INT_WORD
%token FLOAT_WORD
%token BOOLEAN_WORD

%token SEMI_COLON
%token EQUALS

%token LEFT_BRACKET
%token RIGHT_BRACKET
%token LEFT_BRACKET_CURLY
%token RIGHT_BRACKET_CURLY

%token SYSTEM_OUT

%type <sType> primitive_type
%type <expr_type> expression
%type <bexpr_type> bool_expression
%type <stmt_type> statement
%type <stmt_type> statement_list
%type <stmt_type> if
%type <stmt_type> while
%type <stmt_type> for

%type <ival> init_label
%type <ival> goto


%% 
method_body: 
	{	generateHeader();	}
	statement_list
	init_label
	{
		genCode($2.nextList, $3);
		generateFooter();
	}
	;
statement_list: 
	statement 
	| 
	statement
	init_label
	statement_list 
	{
		genCode($1.nextList,$2);
		$$.nextList = $3.nextList;
	}
	;

statement: 
	declaration {$$.nextList = new vector<int>();}
	| if {$$.nextList = $1.nextList;}
	| while {$$.nextList = $1.nextList;}
	| for {$$.nextList = $1.nextList;}
	| assignment {$$.nextList = new vector<int>();}
	| system_print {$$.nextList = new vector<int>();}
	;
declaration: 
	primitive_type IDENTIFIER SEMI_COLON 
	{
		string str($2);
		if($1 == INT_T)
		{
			defineVar(str, INT_T);
		}
		else if ($1 == FLOAT_T)
		{
			defineVar(str, FLOAT_T);
		}
	}
	;
primitive_type: 
	INT_WORD {$$ = INT_T;}
	| FLOAT_WORD {$$ = FLOAT_T;}
	| BOOLEAN_WORD {$$ = BOOL_T;}
	;
	
init_label:
{
	$$ = labelsCount;
	addLineCode(generateLabel() + ":");
}
;
goto:
{
	$$ = codeList.size();
	addLineCode("goto ");
}
;
if: 
	IF_WORD LEFT_BRACKET 
	bool_expression 
	RIGHT_BRACKET LEFT_BRACKET_CURLY 
	init_label
	statement_list
	goto 
	RIGHT_BRACKET_CURLY 
	ELSE_WORD LEFT_BRACKET_CURLY 
	init_label
	statement_list 
	RIGHT_BRACKET_CURLY
	{
		genCode($3.trueList,$6);
		genCode($3.falseList,$12);
		$$.nextList = merge($7.nextList, $13.nextList);
		$$.nextList->push_back($8);
	}
	;
while:
	init_label 
	WHILE_WORD LEFT_BRACKET
	bool_expression
	RIGHT_BRACKET LEFT_BRACKET_CURLY 
	init_label
	statement_list
	RIGHT_BRACKET_CURLY
	{
		addLineCode("goto " + getLabel($1));
		genCode($8.nextList,$1);
		genCode($4.trueList,$7);
		$$.nextList = $4.falseList;
	}
	;
for:
	FOR_WORD 
	LEFT_BRACKET
	assignment
	init_label
	bool_expression
	SEMI_COLON
	init_label
	assignment
	goto
	RIGHT_BRACKET
	LEFT_BRACKET_CURLY
	init_label
	statement_list
	goto
	RIGHT_BRACKET_CURLY
	{
		genCode($5.trueList,$12);
		vector<int> * v = new vector<int> ();
		v->push_back($9);
		genCode(v,$4);
		v = new vector<int>();
		v->push_back($14);
		genCode(v,$7);
		genCode($13.nextList,$7);
		$$.nextList = $5.falseList;
	}
	;
assignment: 
	IDENTIFIER EQUALS expression SEMI_COLON
	{
		string str($1);
		/* After expression finishes, its result should be on top of stack. 
		   Store the top of stack to the identifier
		 */
		if(checkId(str))
		{
			if($3.sType == symbTab[str].second)
			{
				if($3.sType == INT_T)
				{
					addLineCode("istore " + to_string(symbTab[str].first));
				}
				else if ($3.sType == FLOAT_T)
				{
					addLineCode("fstore " + to_string(symbTab[str].first));
				}
			}
			else
			{
				cast(str, $3.sType);	// casting
			}
		}else{
			string err = "identifier: "+str+" isn't declared in this scope";
			yyerror(err.c_str());
		}
	}
	;
expression: 
	FLOAT 	{$$.sType = FLOAT_T; addLineCode("ldc "+to_string($1));}
	| INT 	{$$.sType = INT_T;  addLineCode("ldc "+to_string($1));} 
	| expression ARITH_OP expression	{arithCast($1.sType, $3.sType, string($2));}
	| IDENTIFIER {
		string str($1);
		if(checkId(str))
		{
			$$.sType = symbTab[str].second;
			if(symbTab[str].second == INT_T)
			{
				addLineCode("iload " + to_string(symbTab[str].first));
			}else if (symbTab[str].second == FLOAT_T)
			{
				addLineCode("fload " + to_string(symbTab[str].first));
			}
		}
		else
		{
			string err = "identifier: "+str+" isn't declared in this scope";
			yyerror(err.c_str());
			$$.sType = ERROR_T;
		}
	}
	| LEFT_BRACKET expression RIGHT_BRACKET {$$.sType = $2.sType;}
	;
system_print:
	SYSTEM_OUT LEFT_BRACKET expression RIGHT_BRACKET SEMI_COLON
	{
		if($3.sType == INT_T)
		{
			/* expression is at top of stack now
			   save it at the predefined temp syso var 
			 */			
			addLineCode("istore " + to_string(symbTab["1syso_int_var"].first));
			/* call syso */			
			addLineCode("getstatic      java/lang/System/out Ljava/io/PrintStream;");
			/*insert param*/
			addLineCode("iload " + to_string(symbTab["1syso_int_var"].first ));
			/*invoke syso*/
			addLineCode("invokevirtual java/io/PrintStream/println(I)V");

		}else if ($3.sType == FLOAT_T)
		{
			addLineCode("fstore " + to_string(symbTab["1syso_float_var"].first));
			/* call syso */			
			addLineCode("getstatic      java/lang/System/out Ljava/io/PrintStream;");
			/*insert param*/
			addLineCode("fload " + to_string(symbTab["1syso_float_var"].first ));
			/*invoke syso*/
			addLineCode("invokevirtual java/io/PrintStream/println(F)V");
		}
	}
	;
bool_expression:
	BOOL
	{
		if($1)
		{
			/* bool is 'true' */
			$$.trueList = new vector<int> ();
			$$.trueList->push_back(codeList.size());
			$$.falseList = new vector<int>();
			addLineCode("goto ");
		}else
		{
			$$.trueList = new vector<int> ();
			$$.falseList= new vector<int>();
			$$.falseList->push_back(codeList.size());
			addLineCode("goto ");
		}
	}
	|bool_expression
	BOOL_OP 
	init_label
	bool_expression
	{
		if(!strcmp($2, "&&"))
		{
			genCode($1.trueList, $3);
			$$.trueList = $4.trueList;
			$$.falseList = merge($1.falseList,$4.falseList);
		}
		else if (!strcmp($2,"||"))
		{
			genCode($1.falseList,$3);
			$$.trueList = merge($1.trueList, $4.trueList);
			$$.falseList = $4.falseList;
		}
	}
	| expression RELA_OP expression
	{
		string op ($2);
		$$.trueList = new vector<int>();
		$$.trueList ->push_back (codeList.size());
		$$.falseList = new vector<int>();
		$$.falseList->push_back(codeList.size()+1);
		addLineCode(getOperationName(op)+ " ");
		addLineCode("goto ");
	}
	;
%%


/*-------------------------------Main---------------------------------------------*/

main (int argv, char * argc[])
{
	FILE *myfile;
	if(argv == 1) 
	{
		myfile = fopen("code.txt", "r");
		outfileName = "code.txt";
	}
	else 
	{
		myfile = fopen(argc[1], "r");
		outfileName = string(argc[1]);
	}
	if (!myfile) {
		printf("Code file does not exist!\n");
		char path[200];
		if (!getcwd(path, sizeof(path)))
		     {
		     return -1;
		     }
		printf("%s\n",path);  
		getchar();
		return -1;
	}
	yyin = myfile;
	yyparse();
	printCode();
}

void yyerror(const char * s)
{
	printf("error@%d: %s\n",lineCounter, s);
}

void generateHeader()
{
	addLineCode(".source " + outfileName);
	addLineCode(".class public test\n.super java/lang/Object\n"); //code for defining class
	addLineCode(".method public <init>()V");
	addLineCode("aload_0");
	addLineCode("invokenonvirtual java/lang/Object/<init>()V");
	addLineCode("return");
	addLineCode(".end method\n");
	addLineCode(".method public static main([Ljava/lang/String;)V");
	addLineCode(".limit locals 100\n.limit stack 100");

	/* generate temporal vars for syso*/
	defineVar("1syso_int_var",INT_T);
	defineVar("1syso_float_var",FLOAT_T);

	/*generate line*/
	addLineCode(".line 1");
}

void generateFooter()
{
	addLineCode("return");
	addLineCode(".end method");
}

void cast (string str, int t1)
{
	yyerror("casting not implemented yet :)");
}

void arithCast(int from, int to, string op)
{
	if(from == to)
	{
		if(from == INT_T)
		{
			addLineCode("i" + getOperationName(op));
		}
		else if (from == FLOAT_T)
		{
			addLineCode("f" + getOperationName(op));
		}
	}
	else
	{
		yyerror("cast not implemented yet");
	}
}


string getOperationName(string op)
{
	if(inst_list.find(op) != inst_list.end())
	{
		return inst_list[op];
	}
	return "";
}

bool checkId(string op)
{
	return (symbTab.find(op) != symbTab.end());
}

void defineVar(string name, int type)
{
	if(checkId(name))
	{
		string err = "variable: "+name+" declared before";
		yyerror(err.c_str());
	}
	else
	{
		if(type == INT_T)
		{
			addLineCode("iconst_0\nistore " + to_string(varaiblesNum));
		}
		else if ( type == FLOAT_T)
		{
			addLineCode("fconst_0\nfstore " + to_string(varaiblesNum));
		}
		symbTab[name] = make_pair(varaiblesNum++,(type_enum)type);
	}
}

string generateLabel()
{
	return "L_"+to_string(labelsCount++);
}

string getLabel(int n)
{
	return "L_"+to_string(n);
}

vector<int> * merge(vector<int> *list1, vector<int> *list2)
{
	if(list1 && list2){
		vector<int> *list = new vector<int> (*list1);
		list->insert(list->end(), list2->begin(),list2->end());
		return list;
	}else if(list1)
	{
		return list1;
	}else if (list2)
	{
		return list2;
	}else
	{
		return new vector<int>();
	}
}
void genCode(vector<int> *lists, int ind)
{
	if(! lists) return;
	for(int i = 0 ; i < lists->size() ; i++)
	{
		codeList[(*lists)[i]] = codeList[(*lists)[i]] + getLabel(ind);
	}
}
void addLineCode(string x)
{
	codeList.push_back(x);
}

void printCode(void)
{
	for ( int i = 0 ; i < codeList.size() ; i++)
	{
		fileOut<<codeList[i]<<endl;
	}
}

