%option noyywrap
%{
#include <cmath>
#include <vector>
#include <cstring>
#include "../include/symtab.h"
#include "../parser/SysY_parser.tab.h"

extern int line_number;
extern int col_number;
extern int cur_col_number;
extern IdTable id_table;
void yyerror(char* s, ...);
%}

    /* TODO():增加处理注释的代码*/
%x COMMENT

%%
"//"[^\n]* { // 单行注释处理（不包括换行符）  
    cur_col_number = col_number;
    col_number += strlen(yytext); 
}
"/*" { 
    cur_col_number = col_number;
    col_number += strlen(yytext);
    BEGIN(COMMENT); 
}
<COMMENT>.|\n {
    if (yytext[0] == '\n') {
        ++line_number;
        col_number = 0;
    } else {
        col_number += strlen(yytext);
    }
}
 /* 注释结束，退出 COMMENT 状态，恢复到 INITIAL 状态 */
<COMMENT>"*/" {
    cur_col_number = col_number;
    col_number += strlen(yytext);
    BEGIN(INITIAL);
}
    /* TODO():增加处理列号的代码(cur_col_number表示当前token开始位置, col_number表示当前token结束位置) */
"<=" {cur_col_number=col_number;col_number+=strlen(yytext);return LEQ;}         
">=" {cur_col_number=col_number;col_number+=strlen(yytext);return GEQ;}         
"==" {cur_col_number=col_number;col_number+=strlen(yytext);return EQ;}        
"!=" {cur_col_number=col_number;col_number+=strlen(yytext);return NE;}
"&&" {cur_col_number=col_number;col_number+=strlen(yytext);return AND;}         
"||" {cur_col_number=col_number;col_number+=strlen(yytext);return OR;}               
"const" {cur_col_number=col_number;col_number+=strlen(yytext);return CONST;}     
"if" {cur_col_number=col_number;col_number+=strlen(yytext);return IF;}       
"else" {cur_col_number=col_number;col_number+=strlen(yytext);return ELSE;}  
"while" {cur_col_number=col_number;col_number+=strlen(yytext);return WHILE;}
"void" {cur_col_number=col_number;col_number+=strlen(yytext);return NONE_TYPE;}    
"int" {cur_col_number=col_number;col_number+=strlen(yytext);return INT;}    
"float" {cur_col_number=col_number;col_number+=strlen(yytext);return FLOAT;}      
"return" {cur_col_number=col_number;col_number+=strlen(yytext);return RETURN;}    
"break" {cur_col_number=col_number;col_number+=strlen(yytext);return BREAK;}     
"continue" {cur_col_number=col_number;col_number+=strlen(yytext);return CONTINUE;}

\n {cur_col_number=col_number;++line_number;col_number = 0;}

[ \t\f\r\v] {cur_col_number=col_number;col_number += strlen(yytext);}

[\.\+\-\*\/\=\<\!\%\>] {cur_col_number=col_number;col_number+=strlen(yytext);return yytext[0];}

[\{\}\;\(\)\,\[\]] {cur_col_number=col_number;col_number+=strlen(yytext);return yytext[0];}

[_a-zA-Z][_a-zA-Z0-9]* {
    cur_col_number = col_number;
    col_number += strlen(yytext);
    yylval.symbol_token = id_table.add_id(std::string(yytext));
    return IDENT;
}
([1-9][0-9]*)|0 {
    cur_col_number = col_number;
    col_number += strlen(yytext);
    // yylval.int_token = stoi(std::string(yytext));
    yylval.int_token = 0;
    for(int i = 0;yytext[i];i++){
        yylval.int_token *= 10;
        yylval.int_token += yytext[i] - '0';
    }
    return INT_CONST;
}
    /*
      在词法分析中，你只需要考虑yylval的三种类型，分别为int_token, float_token, symbol_token
      之所以要定义这三种类型，是因为我们只返回这个token的类型是不够的，我们还需要知道这个token的属性
      例如我们需要知道INT_CONST具体的值是多少
    */
    /*TODO():参考SysY2022定义, 处理更多的词法, 如浮点数，十六进制数等*/
    
    /* int Base8 */
"0"[0-7]* {
   // 记录当前列号
    cur_col_number=col_number;
    // 更新列号到当前token的末尾
    col_number+=strlen(yytext);
    // 初始化整数值为0
    yylval.int_token=0;
    // 遍历yytext中的每个字符（从索引1开始，因为索引0是'0'）
    for(int i=1;yytext[i];i++){  // 跳过"0"
    // 将当前整数值乘以8（因为是八进制）
        yylval.int_token *=8;
         // 将当前字符转换为数字并加到整数值上  
        // （'0'的ASCII码是48，所以'3' - '0' = 3）
        yylval.int_token += yytext[i] - '0';
    }
    // 返回整型常量的token类型
    return INT_CONST;
}  
   /* int Base16 */
"0"[xX][0-9a-fA-F]+ {
    cur_col_number = col_number;
    col_number += strlen(yytext);
    yylval.int_token = 0;
    for (int i = 2; yytext[i]; i++) {  // 跳过 "0x" 或 "0X"
        yylval.int_token *= 16;
        if (yytext[i] >= '0' && yytext[i] <= '9') {
            yylval.int_token += yytext[i] - '0';
        } else if (yytext[i] >= 'a' && yytext[i] <= 'f') {
            yylval.int_token += yytext[i] - 'a' + 10;
        } else if (yytext[i] >= 'A' && yytext[i] <= 'F') {
            yylval.int_token += yytext[i] - 'A' + 10;
        }
    }
    return INT_CONST;
}
    /*float Base16*/
"0"[xX](([0-9a-fA-F]+(\.[0-9a-fA-F]*)?)|(\.[0-9a-fA-F]+))[pP][\+\-]?[0-9]+ {
    cur_col_number=col_number;
    col_number+=strlen(yytext);
    int pos_d=0;
    while(yytext[pos_d]){
        if(yytext[pos_d]=='.')break;
        pos_d++;
    } 
    int pos_p=0;
    while(yytext[pos_p]){
        if(yytext[pos_p]=='p'|| yytext[pos_p]=='P')break;
        pos_p++;
    }  
    double temp=0;  
    if(yytext[pos_d]){
        double temp1=0,temp2=0;
        for(int i=2;i<pos_d;i++){
            temp1*=16;
            if(yytext[i]>='0' && yytext[i]<='9')
            	temp1+=yytext[i]-'0';
            else if(yytext[i]>='a' && yytext[i]<='f')
                temp1+=yytext[i]-'a'+10;
            else
            	temp1+=yytext[i]-'A'+10;
        }
        for(int i=pos_p-1;i>pos_d;i--){
            if(yytext[i]>='0' && yytext[i]<='9')
            	temp2+=yytext[i]-'0';
            else if(yytext[i]>='a' && yytext[i]<='f')
                temp2+=yytext[i]-'a'+10;
            else
            	temp2+=yytext[i]-'A'+10;
            temp2/=16;    
        }
        temp=temp1+temp2;
    }
    else{
        for(int i=2;i<pos_p;i++){
            temp*=16;
            if(yytext[i]>='0' && yytext[i]<='9')
            	temp+=yytext[i]-'0';
            else if(yytext[i]>='a' && yytext[i]<='f')
                temp+=yytext[i]-'a'+10;
            else
            	temp+=yytext[i]-'A'+10;
        }
    }
    int shift=0,dir=1;
    if(yytext[pos_p+1]=='+')pos_p+=1;
    else if(yytext[pos_p+1]=='-'){
    	pos_p+=1;
    	dir=-1;
    }
    for(int i=pos_p+1;yytext[i];i++){
        shift *=10;
        shift += yytext[i] - '0';
    }
    shift*=dir;
    yylval.float_token = temp*pow(2.0,shift);
    return FLOAT_CONST;
}
    /*float Base10*/
((([0-9]+\.[0-9]*)|(\.[0-9]+))([eE][\+\-]?[0-9]+)?)|([0-9]+[eE][\+\-]?[0-9]+) {
    cur_col_number=col_number;
    col_number+=strlen(yytext);
    int pos_d=0;
    while(yytext[pos_d]){
        if(yytext[pos_d]=='.')break;
        pos_d++;
    } 
    int pos_e=0;
    while(yytext[pos_e]){
        if(yytext[pos_e]=='e'|| yytext[pos_e]=='E')break;
        pos_e++;
    }     
    double temp=0;
    if(yytext[pos_d]){
        double temp1=0,temp2=0;
        for(int i=0;i<pos_d;i++){
            temp1*=10;
            temp1+=yytext[i]-'0';
        }
        for(int i=pos_e-1;i>pos_d;i--){
            temp2+=yytext[i]-'0';
            temp2/=10;    
        }
        temp=temp1+temp2;
    }
    else{
        for(int i=0;i<pos_e;i++){
	    temp*=10;
            temp+=yytext[i]-'0';   
        }
    }
    if(yytext[pos_e]){
    	int shift=0,dir=1;
    	if(yytext[pos_e+1]=='+')pos_e+=1;
    	else if(yytext[pos_e+1]=='-'){
    		pos_e+=1;
    		dir=-1;
    	}
    	for(int i=pos_e+1;yytext[i];i++){
        	shift *=10;
       		shift += yytext[i] - '0';
    	}
    	shift*=dir;
    	yylval.float_token = temp*pow(10.0,shift);
    }
    else yylval.float_token = temp;
    return FLOAT_CONST;
}

    /*unknown tokens, return ERROR*/
. {
    cur_col_number = col_number;
    col_number += strlen(yytext);
    yylval.error_msg = yytext;
    return ERROR;
}
%%

