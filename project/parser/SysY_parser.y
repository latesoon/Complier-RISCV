%{
#include <fstream>
#include "SysY_tree.h"
#include "type.h"
Program ast_root;

void yyerror(char *s, ...);
int yylex();
int error_num = 0;
extern int line_number;
extern std::ofstream fout;
extern IdTable id_table;
%}
%union{
    char* error_msg;
    Symbol symbol_token;
    double float_token; // 对于SysY的浮点常量，我们需要先以double类型计算，再在语法树节点创建的时候转为float
    int int_token;
    Program program;  
    CompUnit comp_unit;  std::vector<CompUnit>* comps; 
    Decl decl;
    Def def;  std::vector<Def>* defs;
    FuncDef func_def;
    Expression expression;  std::vector<Expression>* expressions;
    Stmt stmt;
    Block block;
    InitVal initval;  std::vector<InitVal>* initvals;
    FuncFParam formal;   std::vector<FuncFParam>* formals;
    BlockItem block_item;   std::vector<BlockItem>* block_items;
}
//declare the terminals
%token <symbol_token> STR_CONST IDENT
%token <float_token> FLOAT_CONST
%token <int_token> INT_CONST
%token LEQ GEQ EQ NE // <=   >=   ==   != 
%token AND OR // &&    ||
%token CONST IF ELSE WHILE NONE_TYPE INT FLOAT FOR
%token RETURN BREAK CONTINUE ERROR TODO

//give the type of nonterminals
%type <program> Program
%type <comp_unit> CompUnit 
%type <comps> Comp_list
%type <decl> Decl VarDecl ConstDecl
%type <def> ConstDef VarDef
%type <defs> ConstDef_list VarDef_list 
%type <func_def> FuncDef 
%type <expression> Exp LOrExp AddExp MulExp RelExp EqExp LAndExp UnaryExp PrimaryExp
%type <expression> ConstExp Lval FuncRParams Cond
%type <expression> IntConst FloatConst
%type <expressions> Exp_list;
%type <stmt> Stmt 
%type <block> Block
%type <block_item> BlockItem
%type <block_items> BlockItem_list
%type <initval> ConstInitVal VarInitVal  
%type <initvals> VarInitVal_list ConstInitVal_list
%type <formal> FuncFParam 
%type <formals> FuncFParams
//do new
%type <expressions> Array_list Constarray_list

// THEN和ELSE用于处理if和else的移进-规约冲突
%precedence THEN
%precedence ELSE
%%
Program 
:Comp_list
{
    @$ = @1;
    ast_root = new __Program($1);
    ast_root->SetLineNumber(line_number);
};

Comp_list
:CompUnit 
{
    $$ = new std::vector<CompUnit>;
    ($$)->push_back($1);
}
|Comp_list CompUnit
{
    ($1)->push_back($2);
    $$ = $1;
};

CompUnit
:Decl{
    $$ = new CompUnit_Decl($1); 
    $$->SetLineNumber(line_number);
}
|FuncDef{
    $$ = new CompUnit_FuncDef($1); 
    $$->SetLineNumber(line_number);
}
;

Decl
:VarDecl{
    $$ = $1; 
    $$->SetLineNumber(line_number);
}
|ConstDecl{
    $$ = $1; 
    $$->SetLineNumber(line_number);
}
;

VarDecl
:INT VarDef_list ';'{
    $$ = new VarDecl(Type::INT,$2); 
    $$->SetLineNumber(line_number);
}
// do float 
|FLOAT VarDef_list ';'{
	$$=new VarDecl(Type::FLOAT,$2);
	$$->SetLineNumber(line_number);
}
;

// TODO(): 考虑变量定义更多情况  

ConstDecl
:CONST INT ConstDef_list ';'{
    $$=new ConstDecl(Type::INT,$3); 
    $$->SetLineNumber(line_number);
}
// do const float
|CONST FLOAT ConstDef_list ';'{
    $$=new ConstDecl(Type::FLOAT,$3); 
    $$->SetLineNumber(line_number);
}
;

// TODO(): 考虑变量定义更多情况  

// do  
VarDef_list
:VarDef{
	$$=new std::vector<Def>{$1}; 
    //$$->SetLineNumber(line_number);
}
|VarDef_list ',' VarDef{
	$$=$1;
	$$->push_back($3);    
    //$$->SetLineNumber(line_number);
}
;

//do
ConstDef_list
:ConstDef{
	$$=new std::vector<Def>{$1};
	//$$->SetLineNumber(line_number);
}
|ConstDef_list ',' ConstDef{
	$$=$1;
	$$->push_back($3); 
    //$$->SetLineNumber(line_number);
}
;

FuncDef
:INT IDENT '(' FuncFParams ')' Block
{
    $$ = new __FuncDef(Type::INT,$2,$4,$6);
    $$->SetLineNumber(line_number);
}
|INT IDENT '(' ')' Block
{
    $$ = new __FuncDef(Type::INT,$2,new std::vector<FuncFParam>(),$5); 
    $$->SetLineNumber(line_number);
}
//do float none_type
|FLOAT IDENT '(' FuncFParams ')' Block
{
	$$=new __FuncDef(Type::FLOAT,$2,$4,$6);
    $$->SetLineNumber(line_number);
}
|NONE_TYPE IDENT '(' FuncFParams ')' Block
{
	$$=new __FuncDef(Type::VOID,$2,$4,$6);
    $$->SetLineNumber(line_number);
}
|FLOAT IDENT '(' ')' Block
{
    $$ = new __FuncDef(Type::FLOAT,$2,new std::vector<FuncFParam>(),$5); 
    $$->SetLineNumber(line_number);
}
|NONE_TYPE IDENT '(' ')' Block
{
    $$ = new __FuncDef(Type::VOID,$2,new std::vector<FuncFParam>(),$5); 
    $$->SetLineNumber(line_number);
}
;
// TODO(): 考虑函数定义更多情况    

//do new
Array_list
:'[' Exp ']'
{
	$$=new std::vector<Expression> {$2};
	//$$->SetLineNumber(line_number);
}
|Array_list '[' Exp ']' 
{
	$$=$1;
	$$->push_back($3);
	//$$->SetLineNumber(line_number);
}
Constarray_list
:'[' ConstExp ']'
{
	$$=new std::vector<Expression> {$2};
	//$$->SetLineNumber(line_number);
}
|Constarray_list '[' ConstExp ']' 
{
	$$=$1;
	$$->push_back($3);
	//$$->SetLineNumber(line_number);
}

VarDef
:IDENT '=' VarInitVal
{$$ = new VarDef($1,nullptr,$3); $$->SetLineNumber(line_number);}
|IDENT
{$$ = new VarDef_no_init($1,nullptr); $$->SetLineNumber(line_number);}
//do list
|IDENT Array_list
{
	$$=new VarDef_no_init($1,$2);
	$$->SetLineNumber(line_number);
}
|IDENT Array_list '=' VarInitVal
{
	$$=new VarDef($1,$2,$4);
	$$->SetLineNumber(line_number);
}
;   
// TODO(): 考虑变量定义更多情况

//do
ConstDef
:IDENT '=' ConstInitVal
{
	$$ = new ConstDef($1,nullptr,$3);
	$$->SetLineNumber(line_number);
}
|IDENT Constarray_list '=' ConstInitVal
{
	$$ = new ConstDef($1,$2,$4);
	$$->SetLineNumber(line_number);
}
;

//do
ConstInitVal
:ConstExp
{
	$$=new ConstInitVal_exp($1);
	$$->SetLineNumber(line_number);
}
|'{' '}'
{
	$$=new ConstInitVal(new std::vector<InitVal>());
	$$->SetLineNumber(line_number);
}
|'{' ConstInitVal_list '}'
{
	$$=new ConstInitVal($2);
	$$->SetLineNumber(line_number);
}
;

//do
VarInitVal
:Exp
{
	$$=new VarInitVal_exp($1);
	$$->SetLineNumber(line_number);
}
|'{' '}'
{
	$$=new VarInitVal(new std::vector<InitVal>());
	$$->SetLineNumber(line_number);
}
|'{' VarInitVal_list '}'
{
	$$=new VarInitVal($2);
	$$->SetLineNumber(line_number);
}
;

//do
ConstInitVal_list
:ConstInitVal{
	$$=new std::vector<InitVal> {$1};
	//$$->SetLineNumber(line_number);
}
|ConstInitVal_list ',' ConstInitVal{
	$$=$1;
	$$->push_back($3);
	//$$->SetLineNumber(line_number);
}
;

//do
VarInitVal_list
:VarInitVal{
	$$=new std::vector<InitVal> {$1};
	//$$->SetLineNumber(line_number);
}
|VarInitVal_list ',' VarInitVal{
	$$=$1;
	$$->push_back($3);
	//$$->SetLineNumber(line_number);
}
;

FuncFParams
:FuncFParam{
    $$ = new std::vector<FuncFParam>;
    ($$)->push_back($1);
}
|FuncFParams ',' FuncFParam{
    ($1)->push_back($3);
    $$ = $1;
}
;

FuncFParam
:INT IDENT{
    $$ = new __FuncFParam(Type::INT,$2,nullptr);
    $$->SetLineNumber(line_number);
}
//do
|FLOAT IDENT{
	$$ = new __FuncFParam(Type::FLOAT,$2,nullptr);
    $$->SetLineNumber(line_number);
}
|INT IDENT '[' ']' Array_list{
	std::vector<Expression>* temp = new std::vector<Expression>{nullptr};
	temp->insert(temp->end(),$5->begin(),$5->end());
	$$ = new __FuncFParam(Type::INT,$2,temp);
    $$->SetLineNumber(line_number);
}
|FLOAT IDENT '[' ']' Array_list{
	std::vector<Expression>* temp = new std::vector<Expression>{nullptr};
	temp->insert(temp->end(),$5->begin(),$5->end());
	$$ = new __FuncFParam(Type::FLOAT,$2,temp);
    $$->SetLineNumber(line_number);
}
|INT IDENT '[' ']'{
	std::vector<Expression>* temp = new std::vector<Expression>{nullptr};
    $$ = new __FuncFParam(Type::INT,$2,temp);
    $$->SetLineNumber(line_number);
}
|FLOAT IDENT '[' ']'{
	std::vector<Expression>* temp = new std::vector<Expression>{nullptr};
    $$ = new __FuncFParam(Type::FLOAT,$2,temp);
    $$->SetLineNumber(line_number);
}
;
// TODO(): 考虑函数形参更多情况

Block
:'{' BlockItem_list '}'{
    $$ = new __Block($2);
    $$->SetLineNumber(line_number);
}
|'{' '}'{
    $$ = new __Block(new std::vector<BlockItem>);
    $$->SetLineNumber(line_number);
}
;

//do
BlockItem_list
:BlockItem{
	$$=new std::vector<BlockItem>{$1};
	//$$->SetLineNumber(line_number);
}
|BlockItem_list BlockItem{
	$$=$1;
	$$->push_back($2);
	//$$->SetLineNumber(line_number);
}
;

//do
BlockItem
:Decl{
	$$ = new BlockItem_Decl($1);
    $$->SetLineNumber(line_number);
}
|Stmt{
	$$ = new BlockItem_Stmt($1);
    $$->SetLineNumber(line_number);
}
;

//DO
Stmt
:Lval '=' Exp ';'{
    $$ = new assign_stmt($1,$3);
    $$->SetLineNumber(line_number);
}
|Exp ';' {
    $$ = new expr_stmt($1);
    $$->SetLineNumber(line_number);
}
|';' {
    $$ = new null_stmt();
    $$->SetLineNumber(line_number);
}
|Block {
    $$ = new block_stmt($1);
    $$->SetLineNumber(line_number);
}
|IF '(' Cond ')' Stmt %prec THEN {
    $$ = new if_stmt($3, $5);
    $$->SetLineNumber(line_number);
}
|IF '(' Cond ')' Stmt ELSE Stmt {
    $$ = new ifelse_stmt($3, $5, $7);
    $$->SetLineNumber(line_number);
}
|WHILE '(' Cond ')' Stmt {
    $$ = new while_stmt($3, $5);
    $$->SetLineNumber(line_number);
}
|BREAK ';' {
    $$ = new break_stmt();
    $$->SetLineNumber(line_number);
}
|CONTINUE ';' {
    $$ = new continue_stmt();
    $$->SetLineNumber(line_number);
}
|RETURN Exp ';' {
    $$ = new return_stmt($2);
    $$->SetLineNumber(line_number);
}
|RETURN ';' {
    $$ = new return_stmt_void();
    $$->SetLineNumber(line_number);
}
;
/*|SWITCH '(' Exp ')' '{' CaseStmtList '}' {
    $$ = new switch_stmt($3, $6);
    $$->SetLineNumber(line_number);
}
;

CaseStmtList
:CaseStmt CaseStmtList {
    ($$)->push_back($1);
    $$ = $2;
}
|CaseStmt {
    $$ = new std::vector<Stmt*>;
    ($$)->push_back($1);
}
;

CaseStmt
:CASE IntConst ':' Stmt {
    $$ = new case_stmt($2, $4);
}
|DEFAULT ':' Stmt {
    $$ = new default_stmt($3);
}
;*/
//Stmt

Exp
:AddExp{$$ = $1; $$->SetLineNumber(line_number);}
;

Cond
:LOrExp{$$ = $1; $$->SetLineNumber(line_number);}
;

//DO
Lval
:IDENT {
    $$ = new Lval($1, nullptr);
    $$->SetLineNumber(line_number);
}
|IDENT Array_list {
    $$ = new Lval($1, $2);
    $$->SetLineNumber(line_number);
}
;//

//DO
PrimaryExp
: '(' Exp ')' {
    $$ = $2;
    $$->SetLineNumber(line_number);
}
| Lval {
    $$=$1;
    $$->SetLineNumber(line_number);
}
| IntConst {
    $$=$1;
    $$->SetLineNumber(line_number);
}
| FloatConst {
    $$=$1;
    $$->SetLineNumber(line_number);
}
;
//

IntConst
:INT_CONST{
    $$ = new IntConst($1);
    $$->SetLineNumber(line_number);
}
;

FloatConst
:FLOAT_CONST{
    $$ = new FloatConst($1);
    $$->SetLineNumber(line_number);
}
;

UnaryExp
:PrimaryExp{$$ = $1;}
|IDENT '(' FuncRParams ')'{
    $$ = new Func_call($1,$3);
    $$->SetLineNumber(line_number);
}
|IDENT '(' ')'{
    // 在sylib.h这个文件中,starttime()是一个宏定义
    // #define starttime() _sysy_starttime(__LINE__)
    // 我们在语法分析中将其替换为_sysy_starttime(line_number)
    // stoptime同理
    if($1->get_string() == "starttime"){
        auto params = new std::vector<Expression>;
        params->push_back(new IntConst(line_number));
        Expression temp = new FuncRParams(params);
        $$ = new Func_call(id_table.add_id("_sysy_starttime"),temp);
        $$->SetLineNumber(line_number);
    }
    else if($1->get_string() == "stoptime"){
        auto params = new std::vector<Expression>;
        params->push_back(new IntConst(line_number));
        Expression temp = new FuncRParams(params);
        $$ = new Func_call(id_table.add_id("_sysy_stoptime"),temp);
        $$->SetLineNumber(line_number);
    }
    else{
        $$ = new Func_call($1,nullptr);
        $$->SetLineNumber(line_number);
    }
}
|'+' UnaryExp{
    $$ = new UnaryExp_plus($2);
    $$->SetLineNumber(line_number);
}
|'-' UnaryExp{
    $$ = new UnaryExp_neg($2);
    $$->SetLineNumber(line_number);
}
|'!' UnaryExp{
    $$ = new UnaryExp_not($2);
    $$->SetLineNumber(line_number);
}
;

//DO
FuncRParams
: Exp_list {
    $$ = new FuncRParams($1);
    $$->SetLineNumber(line_number);
}
;//

//DO

Exp_list
: Exp {
    $$ = new std::vector<Expression>{$1};
    //$$->SetLineNumber(line_number);
}
| Exp_list ',' Exp {
    $$ = $1;
    $$->push_back($3);
    //$$->SetLineNumber(line_number);
}
;//


//DO
MulExp
: UnaryExp {
    $$ = $1;
    $$->SetLineNumber(line_number);
}
| MulExp '*' UnaryExp {
    $$ = new MulExp_mul($1, $3);
    $$->SetLineNumber(line_number);
}
| MulExp '/' UnaryExp {
    $$ = new MulExp_div($1, $3);
    $$->SetLineNumber(line_number);
}
| MulExp '%' UnaryExp {
    $$ = new MulExp_mod($1, $3);
    $$->SetLineNumber(line_number);
}
;
//

AddExp
:MulExp{
    $$ = $1;
    $$->SetLineNumber(line_number);
}
|AddExp '+' MulExp{
    $$ = new AddExp_plus($1,$3); 
    $$->SetLineNumber(line_number);
}
|AddExp '-' MulExp{
    $$ = new AddExp_sub($1,$3); 
    $$->SetLineNumber(line_number);
}
;

//DO
RelExp
: AddExp {
    $$ = $1;
    $$->SetLineNumber(line_number);
}
| RelExp '<' AddExp {
    $$ = new RelExp_lt($1, $3);
    $$->SetLineNumber(line_number);
}
| RelExp '>' AddExp {
    $$ = new RelExp_gt($1, $3);
    $$->SetLineNumber(line_number);
}
| RelExp LEQ AddExp {
    $$ = new RelExp_leq($1, $3);
    $$->SetLineNumber(line_number);
}
| RelExp GEQ AddExp {
    $$ = new RelExp_geq($1, $3);
    $$->SetLineNumber(line_number);
}
;

//DO
EqExp
: RelExp {
    $$ = $1;
    $$->SetLineNumber(line_number);
}
| EqExp EQ RelExp {
    $$ = new EqExp_eq($1, $3);
    $$->SetLineNumber(line_number);
}
| EqExp NE RelExp {
    $$ = new EqExp_neq($1, $3);
    $$->SetLineNumber(line_number);
}
;

//DO
LAndExp
: EqExp {
    $$ = $1;
    $$->SetLineNumber(line_number);
}
| LAndExp AND EqExp {
    $$ = new LAndExp_and($1, $3);
    $$->SetLineNumber(line_number);
}
;

//DO
LOrExp
: LAndExp {
    $$ = $1;
    $$->SetLineNumber(line_number);
}
| LOrExp OR LAndExp {
    $$ = new LOrExp_or($1, $3);
    $$->SetLineNumber(line_number);
}
;
//

//do
ConstExp
:Exp{
	$$=$1;
	$$->SetLineNumber(line_number);
}
;

// TODO: 也许你需要添加更多的非终结符



%% 

void yyerror(char* s, ...)
{
    ++error_num;
    fout<<"parser error in line "<<line_number<<"\n";
}
