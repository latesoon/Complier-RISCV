#include "semant.h"
#include "../include/SysY_tree.h"
#include "../include/ir.h"
#include "../include/type.h"
/*
    语义分析阶段需要对语法树节点上的类型和常量等信息进行标注, 即NodeAttribute类
    同时还需要标注每个变量的作用域信息，即部分语法树节点中的scope变量
    你可以在utils/ast_out.cc的输出函数中找到你需要关注哪些语法树节点中的NodeAttribute类及其他变量
    以及对语义错误的代码输出报错信息
*/

/*
    错误检查的基本要求:
    • 检查 main 函数是否存在 (根据SysY定义，如果不存在main函数应当报错)；
    • 检查未声明变量，及在同一作用域下重复声明的变量；
    • 条件判断和运算表达式：int 和 bool 隐式类型转换（例如int a=5，return a+!a）；
    • 数值运算表达式：运算数类型是否正确 (如返回值为 void 的函数调用结果是否参与了其他表达式的计算)；
    • 检查未声明函数，及函数形参是否与实参类型及数目匹配；
    • 检查是否存在整型变量除以整型常量0的情况 (如对于表达式a/(5-4-1)，编译器应当给出警告或者直接报错)；

    错误检查的进阶要求:
    • 对数组维度进行相应的类型检查 (例如维度是否有浮点数，定义维度时是否为常量等)；
    • 对float进行隐式类型转换以及其他float相关的检查 (例如模运算中是否有浮点类型变量等)；
*/
extern LLVMIR llvmIR;

SemantTable semant_table;
std::vector<std::string> error_msgs{}; // 将语义错误信息保存到该变量中

//add 2024.11.5,2024,11,19
int cntmain=0,cntwhile=0;

void __Program::TypeCheck() {
    semant_table.symbol_table.enter_scope();
    auto comp_vector = *comp_list;
    for (auto comp : comp_vector) {
        comp->TypeCheck();
    }
    
    //add 2024.11.5
    if(!cntmain){
    	error_msgs.push_back("No main function.\n");
    }
    else if(cntmain>1){
    	error_msgs.push_back("Multiple (" + std::to_string(cntmain) + ") main functions.\n");
    }
    
}

void Exp::TypeCheck() {
    addexp->TypeCheck();

    attribute = addexp->attribute;
}

void AddExp_plus::TypeCheck() {
    addexp->TypeCheck();
    mulexp->TypeCheck();
    attribute.V.ConstTag = addexp->attribute.V.ConstTag && mulexp->attribute.V.ConstTag;   
    //TODO("BinaryExp Semant"); 2024.11.6
    Type T1,T2;
    T1.type = addexp->attribute.T.type == Type::BOOL ? Type::INT : addexp->attribute.T.type;
    T2.type = mulexp->attribute.T.type == Type::BOOL ? Type::INT : mulexp->attribute.T.type;
    int val1 = (T1.type == Type::BOOL) ? (addexp->attribute.V.val.BoolVal ? 1 : 0) : ((T1.type == Type::INT) ?(addexp->attribute.V.val.IntVal):0);
    int val2 = (T2.type == Type::BOOL) ? (mulexp->attribute.V.val.BoolVal ? 1 : 0) : ((T2.type == Type::INT) ?(mulexp->attribute.V.val.IntVal):0);
    if (T1.type == Type::INT) {
        if (T2.type == Type::INT) {
            if(attribute.V.ConstTag)
            attribute.V.val.IntVal = val1 + val2;
            attribute.T.type = Type::INT;
        } 
        else if (T2.type == Type::FLOAT) {
            if(attribute.V.ConstTag)
            attribute.V.val.FloatVal = val1 + mulexp->attribute.V.val.FloatVal;
            attribute.T.type = Type::FLOAT;
        } 
        else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    } 
    else if (T1.type == Type::FLOAT) {
        if (T2.type == Type::INT) {
            if(attribute.V.ConstTag)
            attribute.V.val.FloatVal = addexp->attribute.V.val.FloatVal + val2;
            attribute.T.type = Type::FLOAT;
        } 
        else if (T2.type == Type::FLOAT) {
            if(attribute.V.ConstTag)
            attribute.V.val.FloatVal = addexp->attribute.V.val.FloatVal + mulexp->attribute.V.val.FloatVal;
            attribute.T.type = Type::FLOAT;
        } 
        else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    } 
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
}

void AddExp_sub::TypeCheck() {
    addexp->TypeCheck();
    mulexp->TypeCheck();

    //TODO("BinaryExp Semant"); 2024.11.6
    Type T1,T2;
    attribute.V.ConstTag = addexp->attribute.V.ConstTag && mulexp->attribute.V.ConstTag; 
    T1.type = addexp->attribute.T.type == Type::BOOL ? Type::INT : addexp->attribute.T.type;
    T2.type = mulexp->attribute.T.type == Type::BOOL ? Type::INT : mulexp->attribute.T.type;
    int val1 = (T1.type == Type::BOOL) ? (addexp->attribute.V.val.BoolVal ? 1 : 0) : ((T1.type == Type::INT) ?(addexp->attribute.V.val.IntVal):0);
    int val2 = (T2.type == Type::BOOL) ? (mulexp->attribute.V.val.BoolVal ? 1 : 0) : ((T2.type == Type::INT) ?(mulexp->attribute.V.val.IntVal):0);
    if (T1.type == Type::INT) {
        if (T2.type == Type::INT) {
            if(attribute.V.ConstTag)
            attribute.V.val.IntVal = val1 - val2;
            attribute.T.type = Type::INT;
        } 
        else if (T2.type == Type::FLOAT) {
            if(attribute.V.ConstTag)
            attribute.V.val.FloatVal = val1 - mulexp->attribute.V.val.FloatVal;
            attribute.T.type = Type::FLOAT;
        } 
        else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    } 
    else if (T1.type == Type::FLOAT) {
        if (T2.type == Type::INT) {
            if(attribute.V.ConstTag)
            attribute.V.val.FloatVal = addexp->attribute.V.val.FloatVal - val2;
            attribute.T.type = Type::FLOAT;
        } 
        else if (T2.type == Type::FLOAT) {
            if(attribute.V.ConstTag)
            attribute.V.val.FloatVal = addexp->attribute.V.val.FloatVal - mulexp->attribute.V.val.FloatVal;
            attribute.T.type = Type::FLOAT;
        } 
        else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    } 
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
}

void MulExp_mul::TypeCheck() {
    mulexp->TypeCheck();
    unary_exp->TypeCheck();

    //TODO("BinaryExp Semant"); 2024.11.6
    Type T1,T2;
    attribute.V.ConstTag = mulexp->attribute.V.ConstTag && unary_exp->attribute.V.ConstTag;
    T1.type = mulexp->attribute.T.type == Type::BOOL ? Type::INT : mulexp->attribute.T.type;
    T2.type = unary_exp->attribute.T.type == Type::BOOL ? Type::INT : unary_exp->attribute.T.type;
    int val1 = (T1.type == Type::BOOL) ? (mulexp->attribute.V.val.BoolVal ? 1 : 0) : ((T1.type == Type::INT) ? (mulexp->attribute.V.val.IntVal) : 0);
    int val2 = (T2.type == Type::BOOL) ? (unary_exp->attribute.V.val.BoolVal ? 1 : 0) : ((T2.type == Type::INT) ? (unary_exp->attribute.V.val.IntVal) : 0);
    if (T1.type == Type::INT) {
        if (T2.type == Type::INT) {
            if(attribute.V.ConstTag)
            attribute.V.val.IntVal = val1 * val2;
            attribute.T.type = Type::INT;
        } 
        else if (T2.type == Type::FLOAT) {
            if(attribute.V.ConstTag)
            attribute.V.val.FloatVal = val1 * unary_exp->attribute.V.val.FloatVal;
            attribute.T.type = Type::FLOAT;
        }
        else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    } 
    else if (T1.type == Type::FLOAT) {
        if (T2.type == Type::INT) {
            if(attribute.V.ConstTag)
            attribute.V.val.FloatVal = mulexp->attribute.V.val.FloatVal * val2;
            attribute.T.type = Type::FLOAT;
        } 
        else if (T2.type == Type::FLOAT) {
            if(attribute.V.ConstTag)
            attribute.V.val.FloatVal = mulexp->attribute.V.val.FloatVal * unary_exp->attribute.V.val.FloatVal;
            attribute.T.type = Type::FLOAT;
        } 
        else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    } 
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
}

void MulExp_div::TypeCheck() {
    mulexp->TypeCheck();
    unary_exp->TypeCheck();

    //TODO("BinaryExp Semant"); 2024.11.6
    Type T1,T2;
    T1.type = mulexp->attribute.T.type == Type::BOOL ? Type::INT : mulexp->attribute.T.type;
    T2.type = unary_exp->attribute.T.type == Type::BOOL ? Type::INT : unary_exp->attribute.T.type;
    int val1 = (T1.type == Type::BOOL) ? (mulexp->attribute.V.val.BoolVal ? 1 : 0) : ((T1.type == Type::INT) ? (mulexp->attribute.V.val.IntVal) : 0);
    int val2 = (T2.type == Type::BOOL) ? (unary_exp->attribute.V.val.BoolVal ? 1 : 0) : ((T2.type == Type::INT) ? (unary_exp->attribute.V.val.IntVal) : 0);
    attribute.V.ConstTag = mulexp->attribute.V.ConstTag && unary_exp->attribute.V.ConstTag;
    if (T1.type == Type::INT) {
        if (T2.type == Type::INT) {
            //std::cout<< val1 <<' '<<val2;
            if (val2 == 0 && unary_exp->attribute.V.ConstTag)
                error_msgs.push_back("Divide by zero in line " + std::to_string(line_number) + "\n");
            else {
                if(attribute.V.ConstTag)
                attribute.V.val.IntVal = val1 / val2;
                attribute.T.type = Type::INT;
            }
        }
        else if (T2.type == Type::FLOAT) {
            if(attribute.V.ConstTag)
            attribute.V.val.FloatVal = val1 / unary_exp->attribute.V.val.FloatVal;
            attribute.T.type = Type::FLOAT;
        }
        else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    }
    else if (T1.type == Type::FLOAT) {
        if (T2.type == Type::INT) {
            if(attribute.V.ConstTag)
            attribute.V.val.FloatVal = mulexp->attribute.V.val.FloatVal / val2;
            attribute.T.type = Type::FLOAT;
        }
        else if (T2.type == Type::FLOAT) {
            if(attribute.V.ConstTag)
            attribute.V.val.FloatVal = mulexp->attribute.V.val.FloatVal / unary_exp->attribute.V.val.FloatVal;
            attribute.T.type = Type::FLOAT;
        }
        else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    } 
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
}

void MulExp_mod::TypeCheck() {
    mulexp->TypeCheck();
    unary_exp->TypeCheck();

    //TODO("BinaryExp Semant"); 2024.11.6
    Type T1,T2;
    T1.type = mulexp->attribute.T.type == Type::BOOL ? Type::INT : mulexp->attribute.T.type;
    T2.type = unary_exp->attribute.T.type == Type::BOOL ? Type::INT : unary_exp->attribute.T.type;
    attribute.V.ConstTag = mulexp->attribute.V.ConstTag && unary_exp->attribute.V.ConstTag;
    int val1 = (T1.type == Type::BOOL) ? (mulexp->attribute.V.val.BoolVal ? 1 : 0) : ((T1.type == Type::INT) ? (mulexp->attribute.V.val.IntVal) : 0);
    int val2 = (T2.type == Type::BOOL) ? (unary_exp->attribute.V.val.BoolVal ? 1 : 0) : ((T2.type == Type::INT) ? (unary_exp->attribute.V.val.IntVal) : 0);
    if (T1.type == Type::INT && T2.type == Type::INT) {
        if (val2 == 0 && unary_exp->attribute.V.ConstTag) 
            error_msgs.push_back("Modulo by zero in line " + std::to_string(line_number) + "\n");
        else {
            if(attribute.V.ConstTag)
                attribute.V.val.IntVal = val1 % val2;
            attribute.T.type = Type::INT;
        }
    }
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    
}

void RelExp_leq::TypeCheck() {
    relexp->TypeCheck();
    addexp->TypeCheck();

    //TODO("BinaryExp Semant"); 2024.11.10
    Type T1, T2;
    T1.type = relexp->attribute.T.type == Type::BOOL ? Type::INT : relexp->attribute.T.type;
    T2.type = addexp->attribute.T.type == Type::BOOL ? Type::INT : addexp->attribute.T.type;
    int val1 = (T1.type == Type::BOOL) ? (relexp->attribute.V.val.BoolVal ? 1 : 0) : ((T1.type == Type::INT) ? (relexp->attribute.V.val.IntVal) : 0);
    int val2 = (T2.type == Type::BOOL) ? (addexp->attribute.V.val.BoolVal ? 1 : 0) : ((T2.type == Type::INT) ? (addexp->attribute.V.val.IntVal) : 0);
    
    if ((T1.type == Type::INT || T1.type == Type::FLOAT) && (T2.type == Type::INT || T2.type == Type::FLOAT)) {
        attribute.T.type = Type::BOOL;
        attribute.V.val.BoolVal = (T1.type == Type::FLOAT ? relexp->attribute.V.val.FloatVal : val1) <= (T2.type == Type::FLOAT ? addexp->attribute.V.val.FloatVal : val2);
    } 
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    attribute.V.ConstTag = relexp->attribute.V.ConstTag && addexp->attribute.V.ConstTag;
}

void RelExp_lt::TypeCheck() {
    relexp->TypeCheck();
    addexp->TypeCheck();

    //TODO("BinaryExp Semant"); 2024.11.10
    Type T1, T2;
    T1.type = relexp->attribute.T.type == Type::BOOL ? Type::INT : relexp->attribute.T.type;
    T2.type = addexp->attribute.T.type == Type::BOOL ? Type::INT : addexp->attribute.T.type;
    int val1 = (T1.type == Type::BOOL) ? (relexp->attribute.V.val.BoolVal ? 1 : 0) : ((T1.type == Type::INT) ? (relexp->attribute.V.val.IntVal) : 0);
    int val2 = (T2.type == Type::BOOL) ? (addexp->attribute.V.val.BoolVal ? 1 : 0) : ((T2.type == Type::INT) ? (addexp->attribute.V.val.IntVal) : 0);
    
    if ((T1.type == Type::INT || T1.type == Type::FLOAT) && (T2.type == Type::INT || T2.type == Type::FLOAT)) {
        attribute.T.type = Type::BOOL;
        attribute.V.val.BoolVal = (T1.type == Type::FLOAT ? relexp->attribute.V.val.FloatVal : val1) < (T2.type == Type::FLOAT ? addexp->attribute.V.val.FloatVal : val2);
    } 
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    attribute.V.ConstTag = relexp->attribute.V.ConstTag && addexp->attribute.V.ConstTag;
}

void RelExp_geq::TypeCheck() {
    relexp->TypeCheck();
    addexp->TypeCheck();

    //TODO("BinaryExp Semant"); 2024.11.10
    Type T1, T2;
    T1.type = relexp->attribute.T.type == Type::BOOL ? Type::INT : relexp->attribute.T.type;
    T2.type = addexp->attribute.T.type == Type::BOOL ? Type::INT : addexp->attribute.T.type;
    int val1 = (T1.type == Type::BOOL) ? (relexp->attribute.V.val.BoolVal ? 1 : 0) : ((T1.type == Type::INT) ? (relexp->attribute.V.val.IntVal) : 0);
    int val2 = (T2.type == Type::BOOL) ? (addexp->attribute.V.val.BoolVal ? 1 : 0) : ((T2.type == Type::INT) ? (addexp->attribute.V.val.IntVal) : 0);
    
    if ((T1.type == Type::INT || T1.type == Type::FLOAT) && (T2.type == Type::INT || T2.type == Type::FLOAT)) {
        attribute.T.type = Type::BOOL;
        attribute.V.val.BoolVal = (T1.type == Type::FLOAT ? relexp->attribute.V.val.FloatVal : val1) >= (T2.type == Type::FLOAT ? addexp->attribute.V.val.FloatVal : val2);
    } 
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    attribute.V.ConstTag = relexp->attribute.V.ConstTag && addexp->attribute.V.ConstTag;
}

void RelExp_gt::TypeCheck() {
    relexp->TypeCheck();
    addexp->TypeCheck();

    //TODO("BinaryExp Semant"); 2024.11.10
    Type T1, T2;
    T1.type = relexp->attribute.T.type == Type::BOOL ? Type::INT : relexp->attribute.T.type;
    T2.type = addexp->attribute.T.type == Type::BOOL ? Type::INT : addexp->attribute.T.type;
    int val1 = (T1.type == Type::BOOL) ? (relexp->attribute.V.val.BoolVal ? 1 : 0) : ((T1.type == Type::INT) ? (relexp->attribute.V.val.IntVal) : 0);
    int val2 = (T2.type == Type::BOOL) ? (addexp->attribute.V.val.BoolVal ? 1 : 0) : ((T2.type == Type::INT) ? (addexp->attribute.V.val.IntVal) : 0);
    
    if ((T1.type == Type::INT || T1.type == Type::FLOAT) && (T2.type == Type::INT || T2.type == Type::FLOAT)) {
        attribute.T.type = Type::BOOL;
        attribute.V.val.BoolVal = (T1.type == Type::FLOAT ? relexp->attribute.V.val.FloatVal : val1) > (T2.type == Type::FLOAT ? addexp->attribute.V.val.FloatVal : val2);
    } 
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    attribute.V.ConstTag = relexp->attribute.V.ConstTag && addexp->attribute.V.ConstTag;
}

void EqExp_eq::TypeCheck() {
    eqexp->TypeCheck();
    relexp->TypeCheck();

    //TODO("BinaryExp Semant"); 2024.11.10
    Type T1, T2;
    T1.type = eqexp->attribute.T.type == Type::BOOL ? Type::INT : eqexp->attribute.T.type;
    T2.type = relexp->attribute.T.type == Type::BOOL ? Type::INT : relexp->attribute.T.type;
    int val1 = (T1.type == Type::BOOL) ? (eqexp->attribute.V.val.BoolVal ? 1 : 0) : ((T1.type == Type::INT) ? (eqexp->attribute.V.val.IntVal) : 0);
    int val2 = (T2.type == Type::BOOL) ? (relexp->attribute.V.val.BoolVal ? 1 : 0) : ((T2.type == Type::INT) ? (relexp->attribute.V.val.IntVal) : 0);
    
    if ((T1.type == Type::INT || T1.type == Type::FLOAT) && (T2.type == Type::INT || T2.type == Type::FLOAT)) {
        attribute.T.type = Type::BOOL;
        attribute.V.val.BoolVal = (T1.type == Type::FLOAT ? eqexp->attribute.V.val.FloatVal : val1) == (T2.type == Type::FLOAT ? relexp->attribute.V.val.FloatVal : val2);
    } 
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    attribute.V.ConstTag = eqexp->attribute.V.ConstTag && relexp->attribute.V.ConstTag;
}

void EqExp_neq::TypeCheck() {
    eqexp->TypeCheck();
    relexp->TypeCheck();

    //TODO("BinaryExp Semant"); 2024.11.10
    Type T1, T2;
    T1.type = eqexp->attribute.T.type == Type::BOOL ? Type::INT : eqexp->attribute.T.type;
    T2.type = relexp->attribute.T.type == Type::BOOL ? Type::INT : relexp->attribute.T.type;
    int val1 = (T1.type == Type::BOOL) ? (eqexp->attribute.V.val.BoolVal ? 1 : 0) : ((T1.type == Type::INT) ? (eqexp->attribute.V.val.IntVal) : 0);
    int val2 = (T2.type == Type::BOOL) ? (relexp->attribute.V.val.BoolVal ? 1 : 0) : ((T2.type == Type::INT) ? (relexp->attribute.V.val.IntVal) : 0);
    
    if ((T1.type == Type::INT || T1.type == Type::FLOAT) && (T2.type == Type::INT || T2.type == Type::FLOAT)) {
        attribute.T.type = Type::BOOL;
        attribute.V.val.BoolVal = (T1.type == Type::FLOAT ? eqexp->attribute.V.val.FloatVal : val1) != (T2.type == Type::FLOAT ? relexp->attribute.V.val.FloatVal : val2);
    } 
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
    attribute.V.ConstTag = eqexp->attribute.V.ConstTag && relexp->attribute.V.ConstTag;
}

void LAndExp_and::TypeCheck() {
    landexp->TypeCheck();
    eqexp->TypeCheck();

    //TODO("BinaryExp Semant"); 2024.11.10
    attribute.T.type = Type::BOOL;
    attribute.V.ConstTag = landexp->attribute.V.ConstTag && eqexp->attribute.V.ConstTag;
    if ((landexp->attribute.T.type == Type::INT || landexp->attribute.T.type == Type::FLOAT || landexp->attribute.T.type == Type::BOOL) 
       && (eqexp->attribute.T.type == Type::INT || eqexp->attribute.T.type == Type::FLOAT || eqexp->attribute.T.type ==Type::BOOL)) {
        attribute.T.type = Type::BOOL;
        bool val1 = (landexp->attribute.T.type == Type::BOOL) ? landexp->attribute.V.val.BoolVal : 
                ((landexp->attribute.T.type == Type::INT) ? (landexp->attribute.V.val.IntVal != 0) : (landexp->attribute.V.val.FloatVal != 0.0));
    	bool val2 = (eqexp->attribute.T.type == Type::BOOL) ? eqexp->attribute.V.val.BoolVal : 
                ((eqexp->attribute.T.type == Type::INT) ? (eqexp->attribute.V.val.IntVal != 0) : (eqexp->attribute.V.val.FloatVal != 0.0));
        attribute.V.val.BoolVal = val1 && val2;

    }
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
       
}

void LOrExp_or::TypeCheck() {
    lorexp->TypeCheck();
    landexp->TypeCheck();

    //TODO("BinaryExp Semant"); 2024.11.10
    attribute.T.type = Type::BOOL;
    attribute.V.ConstTag = lorexp->attribute.V.ConstTag && landexp->attribute.V.ConstTag;
    if ((lorexp->attribute.T.type == Type::INT || lorexp->attribute.T.type == Type::FLOAT || lorexp->attribute.T.type == Type::BOOL) 
       && (landexp->attribute.T.type == Type::INT || landexp->attribute.T.type == Type::FLOAT || landexp->attribute.T.type ==Type::BOOL)) {
        attribute.T.type = Type::BOOL;
        bool val1 = (lorexp->attribute.T.type == Type::BOOL) ? lorexp->attribute.V.val.BoolVal : 
                ((lorexp->attribute.T.type == Type::INT) ? (lorexp->attribute.V.val.IntVal != 0) : (lorexp->attribute.V.val.FloatVal != 0.0));
    	bool val2 = (landexp->attribute.T.type == Type::BOOL) ? landexp->attribute.V.val.BoolVal : 
                ((landexp->attribute.T.type == Type::INT) ? (landexp->attribute.V.val.IntVal != 0) : (landexp->attribute.V.val.FloatVal != 0.0));
        attribute.V.val.BoolVal = val1 || val2;

    }
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
}

void ConstExp::TypeCheck() {
    addexp->TypeCheck();
    attribute = addexp->attribute;
    if (!attribute.V.ConstTag) {    // addexp is not const
        error_msgs.push_back("Expression is not const " + std::to_string(line_number) + "\n");
    }
}

void Lval::TypeCheck() { 
	//TODO("Lval Semant"); 2024.11.18
	VarAttribute id;
	int sc=semant_table.symbol_table.lookup_scope(name);;
    if (sc>=0) {
        scope=sc;
		id=semant_table.symbol_table.lookup_val(name);
    } 
    else if (semant_table.GlobalTable.find(name) != semant_table.GlobalTable.end()) {
        id=semant_table.GlobalTable[name];
        scope=0;
    } 
    else 
        error_msgs.push_back("Undefined id in line " + std::to_string(line_number) + "\n");
	std::vector<int> arraydim;
    bool dimconst = true;
    if (dims != nullptr) {
    	for (auto it = dims->begin(); it != dims->end(); ++it) {
    		auto dim = *it;
    		dim->TypeCheck();
    		if (dim->attribute.T.type != Type::INT && dim->attribute.T.type != Type::BOOL) {
                error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
            }
            else{
            	int dimnum= (dim->attribute.T.type == Type::BOOL) ? (dim->attribute.V.val.BoolVal ? 1 : 0) :dim->attribute.V.val.IntVal;
            	arraydim.push_back(dimnum);
            }
            dimconst &= dim->attribute.V.ConstTag;
    	}
    }
    //std::cout<<id.dims.size()<<' '<<arraydim.size()<<'\n';
    if (arraydim.size() < id.dims.size()) {
        attribute.V.ConstTag = false;
        attribute.T.type = Type::PTR;
    }
    else if (arraydim.size()==id.dims.size()) {
        attribute.V.ConstTag = id.ConstTag & dimconst;
        //std::cout<<attribute.V.ConstTag<<'\n';
        attribute.T.type = id.type;
        if (attribute.V.ConstTag) {
        	int pos = 0;
   			for (int i=0;i<arraydim.size();i++)
        		pos=pos*id.dims[i]+arraydim[i];
            if (attribute.T.type == Type::INT){
                //std::cout<<id.IntInitVals[pos]<<' ';
                attribute.V.val.IntVal=id.IntInitVals[pos];}
            else if (attribute.T.type == Type::FLOAT) 
                attribute.V.val.FloatVal=id.FloatInitVals[pos];
            else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
        }
    }
    else error_msgs.push_back("Invalid dims in line " + std::to_string(line_number) + "\n");
}

void FuncRParams::TypeCheck() { 
	//TODO("FuncRParams Semant"); 2024.11.18
	if (params != nullptr) {
        //std::cout<<params->size();
		for (auto it = params->begin(); it != params->end(); ++it) {
    		auto param = *it;
            param->TypeCheck();
            //std::cout<<param->attribute.V.val.IntVal;
            if (param->attribute.T.type == Type::VOID) {
                error_msgs.push_back("Void funcr_param in line " + std::to_string(line_number) + "\n");
            }
        }
    }
}

void Func_call::TypeCheck() { 
	//TODO("FunctionCall Semant"); 2024.11.18
	attribute.T.type = semant_table.FunctionTable[name]->return_type;
    attribute.V.ConstTag = false;
    int parnum=0;
    if (funcr_params != nullptr) {
        funcr_params->TypeCheck();
        auto params = ((FuncRParams *)funcr_params)->params;
        parnum=params->size();
    }
    if(semant_table.FunctionTable.find(name) == semant_table.FunctionTable.end())
    	error_msgs.push_back("Undefined function in line " + std::to_string(line_number) + "\n");
    else{
    	auto f=((semant_table.FunctionTable.find(name))->second)->formals;
    	if(f->size() !=parnum)
    		error_msgs.push_back("Function params not match in line " + std::to_string(line_number) + "\n");
    }
}

void UnaryExp_plus::TypeCheck() { 
	//TODO("UnaryExp Semant"); 2024.11.19
	unary_exp->TypeCheck();
	Type T1;
    T1.type = unary_exp->attribute.T.type == Type::BOOL ? Type::INT : unary_exp->attribute.T.type;
    int val1 = (T1.type == Type::BOOL) ? (unary_exp->attribute.V.val.BoolVal ? 1 : 0) : ((T1.type == Type::INT) ? (unary_exp->attribute.V.val.IntVal) : 0);
    if(T1.type==Type::INT)
    	attribute.V.val.IntVal = val1;
    else if (T1.type==Type::FLOAT)
    	attribute.V.val.FloatVal = unary_exp->attribute.V.val.FloatVal;
    else
    	error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
	attribute.T.type=T1.type;
	attribute.V.ConstTag=unary_exp->attribute.V.ConstTag;
}

void UnaryExp_neg::TypeCheck() { 
	//TODO("UnaryExp Semant"); 2024.11.19
	unary_exp->TypeCheck();
	Type T1;
    T1.type = unary_exp->attribute.T.type == Type::BOOL ? Type::INT : unary_exp->attribute.T.type;
    int val1 = (T1.type == Type::BOOL) ? (unary_exp->attribute.V.val.BoolVal ? -1 : 0) : ((T1.type == Type::INT) ? (-(unary_exp->attribute.V.val.IntVal)) : 0);
    if(T1.type==Type::INT)
    	attribute.V.val.IntVal = val1;
    else if (T1.type==Type::FLOAT)
    	attribute.V.val.FloatVal = -(unary_exp->attribute.V.val.FloatVal);
    else
    	error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
	attribute.T.type=T1.type;
	attribute.V.ConstTag=unary_exp->attribute.V.ConstTag;
}

void UnaryExp_not::TypeCheck() { 
	//TODO("UnaryExp Semant"); 2024.11.19
	unary_exp->TypeCheck();
	attribute.T.type = Type::INT;
	attribute.V.ConstTag=unary_exp->attribute.V.ConstTag;
    if (unary_exp->attribute.T.type == Type::INT || unary_exp->attribute.T.type == Type::FLOAT || unary_exp->attribute.T.type == Type::BOOL) {
        attribute.V.val.BoolVal  = (unary_exp->attribute.T.type == Type::BOOL) ? (unary_exp->attribute.V.val.BoolVal ? 0 : 1) : 
                ((unary_exp->attribute.T.type == Type::INT) ? ((unary_exp->attribute.V.val.IntVal == 0)? 1 :0) : ((unary_exp->attribute.V.val.FloatVal == 0.0)?1:0));
    }
    else error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
}

void IntConst::TypeCheck() {
    attribute.T.type = Type::INT;
    attribute.V.ConstTag = true;
    attribute.V.val.IntVal = val;
}

void FloatConst::TypeCheck() {
    attribute.T.type = Type::FLOAT;
    attribute.V.ConstTag = true;
    attribute.V.val.FloatVal = val;
}

void StringConst::TypeCheck() { 
	//TODO("StringConst Semant"); 2024.11.19
	return;
}

void PrimaryExp_branch::TypeCheck() {
    exp->TypeCheck();
    attribute = exp->attribute;
}

void assign_stmt::TypeCheck() { 
	//TODO("AssignStmt Semant"); 2024.11.19
    lval->TypeCheck();
    exp->TypeCheck();
    ((Lval *)lval)->isleft = true; 
    if (exp->attribute.T.type == Type::VOID || lval->attribute.T.type==Type::PTR && exp->attribute.T.type != Type::PTR ||lval->attribute.T.type!=Type::PTR && exp->attribute.T.type == Type::PTR)
        error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
}

void expr_stmt::TypeCheck() {
    exp->TypeCheck();
    attribute = exp->attribute;
}

void block_stmt::TypeCheck() { b->TypeCheck(); }

void ifelse_stmt::TypeCheck() {
    Cond->TypeCheck();
    if (Cond->attribute.T.type == Type::VOID) {
        error_msgs.push_back("if cond type is invalid " + std::to_string(line_number) + "\n");
    }
    ifstmt->TypeCheck();
    elsestmt->TypeCheck();
}

void if_stmt::TypeCheck() {
    Cond->TypeCheck();
    if (Cond->attribute.T.type == Type::VOID) {
        error_msgs.push_back("if cond type is invalid " + std::to_string(line_number) + "\n");
    }
    ifstmt->TypeCheck();
}

void while_stmt::TypeCheck() { 
    //TODO("WhileStmt Semant"); 2024.11.19
    cntwhile++;
    Cond->TypeCheck();
    if (Cond->attribute.T.type == Type::VOID) {
        error_msgs.push_back("While cond type is invalid in line " + std::to_string(line_number) + "\n");
    }
    body->TypeCheck();
    cntwhile--;
}

void continue_stmt::TypeCheck() { 
	//TODO("ContinueStmt Semant"); 2024.11.19
	if(cntwhile<=0)
		error_msgs.push_back("Invalid continue in line " + std::to_string(line_number) + "\n");
}

void break_stmt::TypeCheck() { 
	//TODO("BreakStmt Semant"); 2024.11.19
	if(cntwhile<=0)
		error_msgs.push_back("Invalid break in line " + std::to_string(line_number) + "\n");
}

void return_stmt::TypeCheck() { return_exp->TypeCheck(); }

void return_stmt_void::TypeCheck() {}

void ConstInitVal::TypeCheck() { 
	//TODO("ConstInitVal Semant"); 2024.11.19
	for (auto it = initval->begin(); it != initval->end(); ++it) {
    	auto coninit = *it;
        coninit->TypeCheck();
    }
}

void ConstInitVal_exp::TypeCheck() { 
	//TODO("ConstInitValExp Semant"); 2024.11.19
    //std::cout<<1111;
	exp->TypeCheck();
	if(exp->attribute.T.type != Type::INT && exp->attribute.T.type != Type::FLOAT && exp->attribute.T.type != Type::BOOL)
		error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
	else if(!exp->attribute.V.ConstTag)
		error_msgs.push_back("Must be const in line " + std::to_string(line_number) + "\n");
	attribute = exp->attribute;
}

void VarInitVal::TypeCheck() { 
	//TODO("VarInitVal Semant"); 2024.11.19
	for (auto it = initval->begin(); it != initval->end(); ++it) {
    	auto varinit = *it;
        varinit->TypeCheck();
    }
}

void VarInitVal_exp::TypeCheck() { 
	//TODO("VarInitValExp Semant"); 2024.11.19
	exp->TypeCheck();
	if(exp->attribute.T.type != Type::INT && exp->attribute.T.type != Type::FLOAT && exp->attribute.T.type != Type::BOOL)
		error_msgs.push_back("Invalid type in line " + std::to_string(line_number) + "\n");
	attribute = exp->attribute;
}

//add 2024.11.22
bool VarDef_no_init::constcheck(){
    attribute.V.ConstTag = false;
    return false;
}

bool VarDef::constcheck(){
    attribute.V.ConstTag = false;
    return false;
}

bool ConstDef::constcheck(){
    attribute.V.ConstTag = true;
    return true;
}

void VarDef_no_init::TypeCheck() { 
	//TODO("VarDefNoInit Semant"); 2024.11.19
	int sc=semant_table.symbol_table.get_current_scope();
//	if(semant_table.symbol_table.lookup_scope(name) == sc)
//		error_msgs.push_back("Multiple def in line " + std::to_string(line_number) + "\n");
	attribute.V.ConstTag = false;
    scope = sc;
    if(dims != nullptr){
    	for(auto it = dims->begin(); it != dims->end(); ++it){
    		auto nowdim=*it;
    		nowdim->TypeCheck();
            //std::cout<<nowdim->attribute.V.ConstTag;
    		if (nowdim->attribute.V.ConstTag == false || nowdim->attribute.T.type == Type::FLOAT)
    			error_msgs.push_back("Invalid arraydim in line " + std::to_string(line_number) + "\n");
    		dimnum.push_back(nowdim->attribute.V.val.IntVal);
    	}
    }
}

void VarDef::TypeCheck() { 
	//TODO("VarDef Semant"); 2024.11.19
	int sc=semant_table.symbol_table.get_current_scope();
//	if(semant_table.symbol_table.lookup_scope(name) == sc)
//		error_msgs.push_back("Multiple def in line " + std::to_string(line_number) + "\n");
	attribute.V.ConstTag = false;
    scope = sc;
    if(dims != nullptr){
        //attribute.T.type=Type::PTR;
    	for(auto it = dims->begin(); it != dims->end(); ++it){
    		auto nowdim=*it;
    		nowdim->TypeCheck();
    		if (nowdim->attribute.V.ConstTag == false || nowdim->attribute.T.type == Type::FLOAT)
    			error_msgs.push_back("Invalid arraydim in line " + std::to_string(line_number) + "\n");
    		dimnum.push_back(nowdim->attribute.V.val.IntVal);
    	}
    }
    if (init != nullptr) init->TypeCheck();
}

void ConstDef::TypeCheck() { 
	//TODO("ConstDef Semant"); 2024.11.19
	int sc=semant_table.symbol_table.get_current_scope();
	if(semant_table.symbol_table.lookup_scope(name) == sc)
		error_msgs.push_back("Multiple def in line " + std::to_string(line_number) + "\n");
	attribute.V.ConstTag = true;
    scope = sc;
    if(dims != nullptr){
        //attribute.T.type=Type::PTR;
    	for(auto it = dims->begin(); it != dims->end(); ++it){
    		auto nowdim=*it;
    		nowdim->TypeCheck();
    		if (nowdim->attribute.V.ConstTag == false || nowdim->attribute.T.type == Type::FLOAT)
    			error_msgs.push_back("Invalid arraydim in line " + std::to_string(line_number) + "\n");
    		dimnum.push_back(nowdim->attribute.V.val.IntVal);
    	}
    }
    //std::cout<<111;
    if (init != nullptr) init->TypeCheck();
    else error_msgs.push_back("Need init in line " + std::to_string(line_number) + "\n");
}

void VarDecl::TypeCheck() { 
	//TODO("VarDecl Semant"); 2024.11.19
	for (auto it = var_def_list->begin(); it != var_def_list->end(); ++it) {
    	auto vdef = *it;
    	if(semant_table.symbol_table.lookup_scope(vdef->get_name()) == semant_table.symbol_table.get_current_scope())
			error_msgs.push_back("Multiple def in line " + std::to_string(line_number) + "\n");
        vdef->attribute.T.type = type_decl;
    	vdef->TypeCheck();
    	VarAttribute va;
    	va.ConstTag = vdef->attribute.V.ConstTag;
    	va.type = vdef->attribute.T.type;
    	(va.dims).assign((vdef->dimnum).begin(), (vdef->dimnum).end());
    	semant_table.symbol_table.add_Symbol(vdef->get_name(), va);
    }
}

void ConstDecl::TypeCheck() { 
	//TODO("ConstDecl Semant"); 2024.11.19
	for (auto it = var_def_list->begin(); it != var_def_list->end(); ++it) {
    	auto vdef = *it;
    	if(semant_table.symbol_table.lookup_scope(vdef->get_name()) == semant_table.symbol_table.get_current_scope())
			error_msgs.push_back("Multiple def in line " + std::to_string(line_number) + "\n");
        vdef->attribute.T.type = type_decl;
    	vdef->TypeCheck();
    	VarAttribute va;
    	va.ConstTag = vdef->attribute.V.ConstTag;
    	va.type = vdef->attribute.T.type;
    	(va.dims).assign((vdef->dimnum).begin(), (vdef->dimnum).end());
        if(vdef->get_init()!=nullptr){
            if(va.type==Type::INT){
                //std::cout<<111;
                if(vdef->get_dims() == nullptr)
                    va.IntInitVals.push_back(((vdef->get_init())->get_exp())->attribute.V.val.IntVal);
            }
            else if(va.type ==Type::FLOAT){
                if(vdef->get_dims() == nullptr)
                    va.FloatInitVals.push_back(((vdef->get_init())->get_exp())->attribute.V.val.FloatVal);
            }
        }
    	semant_table.symbol_table.add_Symbol(vdef->get_name(), va);
    }
}

void BlockItem_Decl::TypeCheck() { decl->TypeCheck(); }

void BlockItem_Stmt::TypeCheck() { stmt->TypeCheck(); }

void __Block::TypeCheck() {
    semant_table.symbol_table.enter_scope();
    auto item_vector = *item_list;
    for (auto item : item_vector) {
        item->TypeCheck();
    }
    semant_table.symbol_table.exit_scope();
}

void __FuncFParam::TypeCheck() {
    VarAttribute val;
    val.ConstTag = false;
    val.type = type_decl;
    scope = 1;

    // 如果dims为nullptr, 表示该变量不含数组下标, 如果你在语法分析中采用了其他方式处理，这里也需要更改
    if (dims != nullptr) {    
        auto dim_vector = *dims;

        // the fisrt dim of FuncFParam is empty
        // eg. int f(int A[][30][40])
        val.dims.push_back(-1);    // 这里用-1表示empty，你也可以使用其他方式
        for (int i = 1; i < dim_vector.size(); ++i) {
            auto d = dim_vector[i];
            d->TypeCheck();
            if (d->attribute.V.ConstTag == false) {
                error_msgs.push_back("Array Dim must be const expression in line " + std::to_string(line_number) +
                                     "\n");
            }
            if (d->attribute.T.type == Type::FLOAT) {
                error_msgs.push_back("Array Dim can not be float in line " + std::to_string(line_number) + "\n");
            }
            val.dims.push_back(d->attribute.V.val.IntVal);
        }
        attribute.T.type = Type::PTR;
    } else {
        attribute.T.type = type_decl;
    }

    if (name != nullptr) {
        if (semant_table.symbol_table.lookup_scope(name) != -1) {
            error_msgs.push_back("multiple difinitions of formals in function " + name->get_string() + " in line " +
                                 std::to_string(line_number) + "\n");
        }
        semant_table.symbol_table.add_Symbol(name, val);
    }
}

void __FuncDef::TypeCheck() {
    semant_table.symbol_table.enter_scope();

    semant_table.FunctionTable[name] = this;

    auto formal_vector = *formals;
    for (auto formal : formal_vector) {
        formal->TypeCheck();
    }

    // block TypeCheck
    if (block != nullptr) {
        auto item_vector = *(block->item_list);
        for (auto item : item_vector) {
            item->TypeCheck();
        }
    }
	
	//add 2024.11.5
	if (name->get_string()=="main") cntmain++;
	
    semant_table.symbol_table.exit_scope();
}
/*
//ref begin
int FindMinDimStep(const VarAttribute &val, int relativePos, int dimsIdx, int &max_subBlock_sz) {
    int min_dim_step = 1;
    int blockSz = 1;
    for (int i = dimsIdx + 1; i < val.dims.size(); i++) {
        blockSz *= val.dims[i];
    }
    while (relativePos % blockSz != 0) {
        min_dim_step++;
        blockSz /= val.dims[dimsIdx + min_dim_step - 1];
    }
    max_subBlock_sz = blockSz;
    return min_dim_step;
}

void RecursiveArrayInit(InitVal init, VarAttribute &val, int begPos, int endPos, int dimsIdx) {
    // dimsIdx from 0
    int pos = begPos;

    // Old Policy: One { } for one dim

    for (InitVal iv : *(init->get_initval())) {
        if (iv->get_exp()) {
            if (iv->attribute.T.type == Type::VOID) {
                error_msgs.push_back("exp can not be void in initval in line " + std::to_string(init->GetLineNumber()) +
                                     "\n");
            }
            if (val.type == Type::INT) {
                if (iv->attribute.T.type == Type::INT) {
                    val.IntInitVals[pos] = iv->attribute.V.val.IntVal;
                } else if (iv->attribute.T.type == Type::FLOAT) {
                    val.IntInitVals[pos] = iv->attribute.V.val.FloatVal;
                }
            }
            if (val.type == Type::FLOAT) {
                if (iv->attribute.T.type == Type::INT) {
                    val.FloatInitVals[pos] = iv->attribute.V.val.IntVal;
                } else if (iv->attribute.T.type == Type::FLOAT) {
                    val.FloatInitVals[pos] = iv->attribute.V.val.FloatVal;
                }
            }
            pos++;
        } else {
            // New Policy: One { } for the max align-able dim
            // More informations see comments above FindMinDimStep
            int max_subBlock_sz = 0;
            int min_dim_step = FindMinDimStep(val, pos - begPos, dimsIdx, max_subBlock_sz);
            RecursiveArrayInit(iv, val, pos, pos + max_subBlock_sz - 1, dimsIdx + min_dim_step);
            pos += max_subBlock_sz;
        }
    }
}

void SolveIntInitVal(InitVal init, VarAttribute &val)    // used for global or const
{
    val.type = Type::INT;
    int arraySz = 1;
    for (auto d : val.dims) {
        arraySz *= d;
    }
    val.IntInitVals.resize(arraySz, 0);
    if (val.dims.empty()) {
        if (init->get_exp() != nullptr) {
            if (init->get_exp()->attribute.T.type == Type::VOID) {
                error_msgs.push_back("Expression can not be void in initval in line " +
                                     std::to_string(init->GetLineNumber()) + "\n");
            } else if (init->get_exp()->attribute.T.type == Type::INT) {
                val.IntInitVals[0] = init->get_exp()->attribute.V.val.IntVal;
            } else if (init->get_exp()->attribute.T.type == Type::FLOAT) {
                val.IntInitVals[0] = init->get_exp()->attribute.V.val.FloatVal;
            }
        }
        return;
    } else {
        if (init->get_exp()) {
            if ((init)->get_exp() != nullptr) {
                error_msgs.push_back("InitVal can not be exp in line " + std::to_string(init->GetLineNumber()) + "\n");
            }
            return;
        } else {
            RecursiveArrayInit(init, val, 0, arraySz - 1, 0);
        }
    }
}

void SolveFloatInitVal(InitVal init, VarAttribute &val)    // used for global or const
{
    val.type = Type::FLOAT;
    int arraySz = 1;
    for (auto d : val.dims) {
        arraySz *= d;
    }
    val.FloatInitVals.resize(arraySz, 0);
    if (val.dims.empty()) {
        if (init->get_exp() != nullptr) {
            if (init->get_exp()->attribute.T.type == Type::VOID) {
                error_msgs.push_back("exp can not be void in initval in line " + std::to_string(init->GetLineNumber()) +
                                     "\n");
            } else if (init->get_exp()->attribute.T.type == Type::FLOAT) {
                val.FloatInitVals[0] = init->get_exp()->attribute.V.val.FloatVal;
            } else if (init->get_exp()->attribute.T.type == Type::INT) {
                val.FloatInitVals[0] = init->get_exp()->attribute.V.val.IntVal;
            }
        }
        return;
    } else {
        if (init->get_exp()) {
            if ((init)->get_exp() != nullptr) {
                error_msgs.push_back("InitVal can not be exp in line " + std::to_string(init->GetLineNumber()) + "\n");
            }
            return;
        } else {
            RecursiveArrayInit(init, val, 0, arraySz - 1, 0);
        }
    }
}

// int b = a[3][4]
int GetArrayIntVal(VarAttribute &val, std::vector<int> &indexs) {
    //[i] + i
    //[i][j] + i*dim[1] + j
    //[i][j][k] + i*dim[1]*dim[2] + j*dim[2] + k
    //[i][j][k][w] + i*dim[1]*dim[2]*dim[3] + j*dim[2]*dim[3] + k*dim[3] + w
    int idx = 0;
    for (int curIndex = 0; curIndex < indexs.size(); curIndex++) {
        idx *= val.dims[curIndex];
        idx += indexs[curIndex];
    }
    return val.IntInitVals[idx];
}

float GetArrayFloatVal(VarAttribute &val, std::vector<int> &indexs) {
    int idx = 0;
    for (int curIndex = 0; curIndex < indexs.size(); curIndex++) {
        idx *= val.dims[curIndex];
        idx += indexs[curIndex];
    }
    return val.FloatInitVals[idx];
}
//ref end*/

void CompUnit_Decl::TypeCheck() { 
	//TODO("CompUnitDecl Semant"); 2024.11.20
	Type t1;
	t1.type=decl->get_type_decl();
	for (auto it = (decl->get_var_def_list())->begin(); it != (decl->get_var_def_list())->end(); ++it) {
    	auto vdef = *it;
    	if (semant_table.GlobalTable.find(vdef->get_name()) != semant_table.GlobalTable.end())
            error_msgs.push_back("Multiple def in line " + std::to_string(line_number) + "\n");
        //vdef->TypeCheck();
        vdef->attribute.T.type = decl->get_type_decl();
    	VarAttribute va;
    	//va.ConstTag = vdef->attribute.V.ConstTag;
        va.ConstTag = vdef->constcheck();
    	va.type = t1.type;
        vdef->scope=0;
        if(vdef->get_dims() != nullptr){
            //va.type=Type::PTR;
            for(auto it = (vdef->get_dims())->begin(); it != (vdef->get_dims())->end(); ++it){
                auto nowdim=*it;
                nowdim->TypeCheck();
                //std::cout<<nowdim->attribute.V.ConstTag;
                if (nowdim->attribute.V.ConstTag == false || nowdim->attribute.T.type == Type::FLOAT)
                    error_msgs.push_back("Invalid arraydim in line " + std::to_string(line_number) + "\n");
                vdef->dimnum.push_back(nowdim->attribute.V.val.IntVal);
                va.dims.push_back(nowdim->attribute.V.val.IntVal);
            }
        }
        if (vdef->get_init() != nullptr){
            (vdef->get_init())->TypeCheck();
            if(t1.type==Type::INT){
                //std::cout<<111;
                if(vdef->get_dims() == nullptr){
                    int val;
                    auto exp=vdef->get_init()->get_exp();
                    if(exp){
                        if(exp->attribute.T.type==Type::FLOAT)
                            val=int(exp->attribute.V.val.FloatVal);
                        else if(exp->attribute.T.type==Type::INT)
                            val=exp->attribute.V.val.IntVal;
                        else if(exp->attribute.T.type==Type::BOOL)
                            val=(exp->attribute.V.val.BoolVal)?1:0;
                    }
                    va.IntInitVals.push_back(val);
                    //va.IntInitVals.push_back(((vdef->get_init())->get_exp())->attribute.V.val.IntVal);
                }
                /*else{
                    auto dim_vector = *vdef->get_dims();
                    for (auto d : dim_vector) {
                        d->TypeCheck();
                        va.dims.push_back(d->attribute.V.val.IntVal);
                    }
                    InitVal init = vdef->get_init();
                    init->TypeCheck();
                    SolveIntInitVal(init, va);
                }*/   
            }
            else if(t1.type ==Type::FLOAT){
                if(vdef->get_dims() == nullptr){
                    float val;
                    auto exp=vdef->get_init()->get_exp();
                    if(exp){
                        if(exp->attribute.T.type==Type::FLOAT)
                            val=exp->attribute.V.val.FloatVal;
                        else if(exp->attribute.T.type==Type::INT)
                            val=float(exp->attribute.V.val.IntVal);
                        else if(exp->attribute.T.type==Type::BOOL)
                            val=(exp->attribute.V.val.BoolVal)?1.0:0.0;
                    }
                    va.FloatInitVals.push_back(val);
                    //va.FloatInitVals.push_back(((vdef->get_init())->get_exp())->attribute.V.val.FloatVal);
                }
                /*else{
                    auto dim_vector = *vdef->get_dims();
                    for (auto d : dim_vector) {
                        d->TypeCheck();
                        va.dims.push_back(d->attribute.V.val.IntVal);
                    }
                    InitVal init = vdef->get_init();
                    init->TypeCheck();
                    SolveFloatInitVal(init, va);
                }*/ 
            }
        }
    	semant_table.GlobalTable[vdef->get_name()] = va;
    }
	
}

void CompUnit_FuncDef::TypeCheck() { func_def->TypeCheck(); }
