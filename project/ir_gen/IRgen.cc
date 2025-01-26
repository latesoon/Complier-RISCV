#include "IRgen.h"
#include "../include/ir.h"
#include "semant.h"

extern SemantTable semant_table;    // 也许你会需要一些语义分析的信息

IRgenTable irgen_table;    // 中间代码生成的辅助变量
LLVMIR llvmIR;             // 我们需要在这个变量中生成中间代码

//add 2024.11.21
int nowreg=-1;
int nowlabel=0;
int totlabel=-1;
FuncDefInstruction nowfunc;
int whilelabel[2]={-1,-1};
Type::ty func_ret=Type::VOID;

void AddLibFunctionDeclare();

// 在基本块B末尾生成一条新指令
void IRgenArithmeticI32(LLVMBlock B, BasicInstruction::LLVMIROpcode opcode, int reg1, int reg2, int result_reg);
void IRgenArithmeticF32(LLVMBlock B, BasicInstruction::LLVMIROpcode opcode, int reg1, int reg2, int result_reg);
void IRgenArithmeticI32ImmLeft(LLVMBlock B, BasicInstruction::LLVMIROpcode opcode, int val1, int reg2, int result_reg);
void IRgenArithmeticF32ImmLeft(LLVMBlock B, BasicInstruction::LLVMIROpcode opcode, float val1, int reg2,
                               int result_reg);
void IRgenArithmeticI32ImmAll(LLVMBlock B, BasicInstruction::LLVMIROpcode opcode, int val1, int val2, int result_reg);
void IRgenArithmeticF32ImmAll(LLVMBlock B, BasicInstruction::LLVMIROpcode opcode, float val1, float val2,
                              int result_reg);

void IRgenIcmp(LLVMBlock B, BasicInstruction::IcmpCond cmp_op, int reg1, int reg2, int result_reg);
void IRgenFcmp(LLVMBlock B, BasicInstruction::FcmpCond cmp_op, int reg1, int reg2, int result_reg);
void IRgenIcmpImmRight(LLVMBlock B, BasicInstruction::IcmpCond cmp_op, int reg1, int val2, int result_reg);
void IRgenFcmpImmRight(LLVMBlock B, BasicInstruction::FcmpCond cmp_op, int reg1, float val2, int result_reg);

void IRgenFptosi(LLVMBlock B, int src, int dst);
void IRgenSitofp(LLVMBlock B, int src, int dst);
void IRgenZextI1toI32(LLVMBlock B, int src, int dst);

void IRgenGetElementptrIndexI32(LLVMBlock B, BasicInstruction::LLVMType type, int result_reg, Operand ptr,
                        std::vector<int> dims, std::vector<Operand> indexs);

void IRgenGetElementptrIndexI64(LLVMBlock B, BasicInstruction::LLVMType type, int result_reg, Operand ptr,
                        std::vector<int> dims, std::vector<Operand> indexs);

void IRgenLoad(LLVMBlock B, BasicInstruction::LLVMType type, int result_reg, Operand ptr);
void IRgenStore(LLVMBlock B, BasicInstruction::LLVMType type, int value_reg, Operand ptr);
void IRgenStore(LLVMBlock B, BasicInstruction::LLVMType type, Operand value, Operand ptr);

void IRgenCall(LLVMBlock B, BasicInstruction::LLVMType type, int result_reg,
               std::vector<std::pair<enum BasicInstruction::LLVMType, Operand>> args, std::string name);
void IRgenCallVoid(LLVMBlock B, BasicInstruction::LLVMType type,
                   std::vector<std::pair<enum BasicInstruction::LLVMType, Operand>> args, std::string name);

void IRgenCallNoArgs(LLVMBlock B, BasicInstruction::LLVMType type, int result_reg, std::string name);
void IRgenCallVoidNoArgs(LLVMBlock B, BasicInstruction::LLVMType type, std::string name);

void IRgenRetReg(LLVMBlock B, BasicInstruction::LLVMType type, int reg);
void IRgenRetImmInt(LLVMBlock B, BasicInstruction::LLVMType type, int val);
void IRgenRetImmFloat(LLVMBlock B, BasicInstruction::LLVMType type, float val);
void IRgenRetVoid(LLVMBlock B);

void IRgenBRUnCond(LLVMBlock B, int dst_label);
void IRgenBrCond(LLVMBlock B, int cond_reg, int true_label, int false_label);

void IRgenAlloca(LLVMBlock B, BasicInstruction::LLVMType type, int reg);
void IRgenAllocaArray(LLVMBlock B, BasicInstruction::LLVMType type, int reg, std::vector<int> dims);

RegOperand *GetNewRegOperand(int RegNo);

// generate TypeConverse Instructions from type_src to type_dst
// eg. you can use fptosi instruction to converse float to int
// eg. you can use zext instruction to converse bool to int

//change 2024.11.21
void IRgenTypeConversetobool(LLVMBlock bl, Type::ty type_src) {
    //TODO("IRgenTypeConverse. Implement it if you need it");
    if (type_src==Type::INT)
        IRgenIcmpImmRight(bl, BasicInstruction::ne, nowreg, 0, nowreg+1);
    else
        IRgenFcmpImmRight(bl, BasicInstruction::ONE, nowreg, 0, nowreg+1); 
    nowreg++;
}

void BasicBlock::InsertInstruction(int pos, Instruction Ins) {
    assert(pos == 0 || pos == 1);
    if (pos == 0) {
        Instruction_list.push_front(Ins);
    } else if (pos == 1) {
        Instruction_list.push_back(Ins);
    }
}

/*
二元运算指令生成的伪代码：
    假设现在的语法树节点是：AddExp_plus
    该语法树表示 addexp + mulexp

    addexp->codeIR()
    mulexp->codeIR()
    假设mulexp生成完后，我们应该在基本块B0继续插入指令。
    addexp的结果存储在r0寄存器中，mulexp的结果存储在r1寄存器中
    生成一条指令r2 = r0 + r1，并将该指令插入基本块B0末尾。
    标注后续应该在基本块B0插入指令，当前节点的结果寄存器为r2。
    (如果考虑支持浮点数，需要查看语法树节点的类型来判断此时是否需要隐式类型转换)
*/

/*
while语句指令生成的伪代码：
    while的语法树节点为while(cond)stmt

    假设当前我们应该在B0基本块开始插入指令
    新建三个基本块Bcond，Bbody，Bend
    在B0基本块末尾插入一条无条件跳转指令，跳转到Bcond

    设置当前我们应该在Bcond开始插入指令
    cond->codeIR()    //在调用该函数前你可能需要设置真假值出口
    假设cond生成完后，我们应该在B1基本块继续插入指令，Bcond的结果为r0
    如果r0的类型不为bool，在B1末尾生成一条比较语句，比较r0是否为真。
    在B1末尾生成一条条件跳转语句，如果为真，跳转到Bbody，如果为假，跳转到Bend

    设置当前我们应该在Bbody开始插入指令
    stmt->codeIR()
    假设当stmt生成完后，我们应该在B2基本块继续插入指令
    在B2末尾生成一条无条件跳转语句，跳转到Bcond

    设置当前我们应该在Bend开始插入指令
*/

void __Program::codeIR() {
    AddLibFunctionDeclare();
    auto comp_vector = *comp_list;
    for (auto comp : comp_vector) {
        //std::cout<<"code";
        comp->codeIR();
    }
}

void Exp::codeIR() { addexp->codeIR(); }

void AddExp_plus::codeIR() { 
	//TODO("BinaryExp CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
	addexp->codeIR();
    int rega=nowreg;
    if(addexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, rega, ++nowreg);
        rega=nowreg;
    }
    if(addexp->attribute.T.type != Type::FLOAT && attribute.T.type == Type::FLOAT){
        IRgenSitofp(bl, rega, ++nowreg);
        rega=nowreg;
    }
    mulexp->codeIR();
    int regb=nowreg;
    if(mulexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(mulexp->attribute.T.type != Type::FLOAT && attribute.T.type == Type::FLOAT){
        IRgenSitofp(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(attribute.T.type == Type::INT)
        IRgenArithmeticI32(bl, BasicInstruction::ADD, rega, regb, ++nowreg);
    else
        IRgenArithmeticF32(bl, BasicInstruction::FADD, rega, regb, ++nowreg);
}

void AddExp_sub::codeIR() { 
    //TODO("BinaryExp CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
	addexp->codeIR();
    int rega=nowreg;
    if(addexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, rega, ++nowreg);
        rega=nowreg;
    }
    if(addexp->attribute.T.type != Type::FLOAT && attribute.T.type == Type::FLOAT){
        IRgenSitofp(bl, rega, ++nowreg);
        rega=nowreg;
    }
    mulexp->codeIR();
    int regb=nowreg;
    if(mulexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(mulexp->attribute.T.type != Type::FLOAT && attribute.T.type == Type::FLOAT){
        IRgenSitofp(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(attribute.T.type == Type::INT)
        IRgenArithmeticI32(bl, BasicInstruction::SUB, rega, regb, ++nowreg);
    else
        IRgenArithmeticF32(bl, BasicInstruction::FSUB, rega, regb, ++nowreg);
}

void MulExp_mul::codeIR() { 
    //TODO("BinaryExp CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
	mulexp->codeIR();
    int rega=nowreg;
    if(mulexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, rega, ++nowreg);
        rega=nowreg;
    }
    if(mulexp->attribute.T.type != Type::FLOAT && attribute.T.type == Type::FLOAT){
        IRgenSitofp(bl, rega, ++nowreg);
        rega=nowreg;
    }
    unary_exp->codeIR();
    int regb=nowreg;
    if(unary_exp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(unary_exp->attribute.T.type != Type::FLOAT && attribute.T.type == Type::FLOAT){
        IRgenSitofp(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(attribute.T.type == Type::INT)
        IRgenArithmeticI32(bl, BasicInstruction::MUL, rega, regb, ++nowreg);
    else
        IRgenArithmeticF32(bl, BasicInstruction::FMUL, rega, regb, ++nowreg);
}

void MulExp_div::codeIR() { 
    //TODO("BinaryExp CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
	mulexp->codeIR();
    int rega=nowreg;
    if(mulexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, rega, ++nowreg);
        rega=nowreg;
    }
    if(mulexp->attribute.T.type != Type::FLOAT && attribute.T.type == Type::FLOAT){
        IRgenSitofp(bl, rega, ++nowreg);
        rega=nowreg;
    }
    unary_exp->codeIR();
    int regb=nowreg;
    if(unary_exp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(unary_exp->attribute.T.type != Type::FLOAT && attribute.T.type == Type::FLOAT){
        IRgenSitofp(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(attribute.T.type == Type::INT)
        IRgenArithmeticI32(bl, BasicInstruction::DIV, rega, regb, ++nowreg);
    else
        IRgenArithmeticF32(bl, BasicInstruction::FDIV, rega, regb, ++nowreg);
}

void MulExp_mod::codeIR() { 
    //TODO("BinaryExp CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
	mulexp->codeIR();
    int rega=nowreg;
    if(mulexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, rega, ++nowreg);
        rega=nowreg;
    }
    unary_exp->codeIR();
    int regb=nowreg;
    if(unary_exp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, regb, ++nowreg);
        regb=nowreg;
    }
    IRgenArithmeticI32(bl, BasicInstruction::MOD, rega, regb, ++nowreg);
}

void RelExp_leq::codeIR() { 
    //TODO("BinaryExp CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
	relexp->codeIR();
    int rega=nowreg;
    bool isfloat=(relexp->attribute.T.type == Type::FLOAT || addexp->attribute.T.type == Type::FLOAT);
    if(relexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, rega, ++nowreg);
        rega=nowreg;
    }
    if(relexp->attribute.T.type != Type::FLOAT && isfloat){
        IRgenSitofp(bl, rega, ++nowreg);
        rega=nowreg;
    }
    addexp->codeIR();
    int regb=nowreg;
    if(addexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(addexp->attribute.T.type != Type::FLOAT && isfloat){
        IRgenSitofp(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(isfloat)
        IRgenFcmp(bl, BasicInstruction::OLE, rega, regb, ++nowreg);
    else
        IRgenIcmp(bl, BasicInstruction::sle, rega, regb, ++nowreg);
}

void RelExp_lt::codeIR() { 
    //TODO("BinaryExp CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
	relexp->codeIR();
    int rega=nowreg;
    bool isfloat=(relexp->attribute.T.type == Type::FLOAT || addexp->attribute.T.type == Type::FLOAT);
    if(relexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, rega, ++nowreg);
        rega=nowreg;
    }
    if(relexp->attribute.T.type != Type::FLOAT && isfloat){
        IRgenSitofp(bl, rega, ++nowreg);
        rega=nowreg;
    }
    addexp->codeIR();
    int regb=nowreg;
    if(addexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(addexp->attribute.T.type != Type::FLOAT && isfloat){
        IRgenSitofp(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(isfloat)
        IRgenFcmp(bl, BasicInstruction::OLT, rega, regb, ++nowreg);
    else
        IRgenIcmp(bl, BasicInstruction::slt, rega, regb, ++nowreg);
}

void RelExp_geq::codeIR() { 
    //TODO("BinaryExp CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
	relexp->codeIR();
    int rega=nowreg;
    bool isfloat=(relexp->attribute.T.type == Type::FLOAT || addexp->attribute.T.type == Type::FLOAT);
    if(relexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, rega, ++nowreg);
        rega=nowreg;
    }
    if(relexp->attribute.T.type != Type::FLOAT && isfloat){
        IRgenSitofp(bl, rega, ++nowreg);
        rega=nowreg;
    }
    addexp->codeIR();
    int regb=nowreg;
    if(addexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(addexp->attribute.T.type != Type::FLOAT && isfloat){
        IRgenSitofp(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(isfloat)
        IRgenFcmp(bl, BasicInstruction::OGE, rega, regb, ++nowreg);
    else
        IRgenIcmp(bl, BasicInstruction::sge, rega, regb, ++nowreg);
}

void RelExp_gt::codeIR() { 
    //TODO("BinaryExp CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
	relexp->codeIR();
    int rega=nowreg;
    bool isfloat=(relexp->attribute.T.type == Type::FLOAT || addexp->attribute.T.type == Type::FLOAT);
    if(relexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, rega, ++nowreg);
        rega=nowreg;
    }
    if(relexp->attribute.T.type != Type::FLOAT && isfloat){
        IRgenSitofp(bl, rega, ++nowreg);
        rega=nowreg;
    }
    addexp->codeIR();
    int regb=nowreg;
    if(addexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(addexp->attribute.T.type != Type::FLOAT && isfloat){
        IRgenSitofp(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(isfloat)
        IRgenFcmp(bl, BasicInstruction::OGT, rega, regb, ++nowreg);
    else
        IRgenIcmp(bl, BasicInstruction::sgt, rega, regb, ++nowreg);
}

void EqExp_eq::codeIR() { 
    //TODO("BinaryExp CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
	eqexp->codeIR();
    int rega=nowreg;
    bool isfloat=(eqexp->attribute.T.type == Type::FLOAT || relexp->attribute.T.type == Type::FLOAT);
    if(eqexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, rega, ++nowreg);
        rega=nowreg;
    }
    if(eqexp->attribute.T.type != Type::FLOAT && isfloat){
        IRgenSitofp(bl, rega, ++nowreg);
        rega=nowreg;
    }
    relexp->codeIR();
    int regb=nowreg;
    if(relexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(relexp->attribute.T.type != Type::FLOAT && isfloat){
        IRgenSitofp(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(isfloat)
        IRgenFcmp(bl, BasicInstruction::OEQ, rega, regb, ++nowreg);
    else
        IRgenIcmp(bl, BasicInstruction::eq, rega, regb, ++nowreg);
}

void EqExp_neq::codeIR() { 
    //TODO("BinaryExp CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
	eqexp->codeIR();
    int rega=nowreg;
    bool isfloat=(eqexp->attribute.T.type == Type::FLOAT || relexp->attribute.T.type == Type::FLOAT);
    if(eqexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, rega, ++nowreg);
        rega=nowreg;
    }
    if(eqexp->attribute.T.type != Type::FLOAT && isfloat){
        IRgenSitofp(bl, rega, ++nowreg);
        rega=nowreg;
    }
    relexp->codeIR();
    int regb=nowreg;
    if(relexp->attribute.T.type == Type::BOOL){
        IRgenZextI1toI32(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(relexp->attribute.T.type != Type::FLOAT && isfloat){
        IRgenSitofp(bl, regb, ++nowreg);
        regb=nowreg;
    }
    if(isfloat)
        IRgenFcmp(bl, BasicInstruction::ONE, rega, regb, ++nowreg);
    else
        IRgenIcmp(bl, BasicInstruction::ne, rega, regb, ++nowreg);
}

// short circuit &&
void LAndExp_and::codeIR() { 
    //TODO("LAndExpAnd CodeIR"); 2024.11.21
    landexp->truelabel=llvmIR.NewBlock(nowfunc, ++totlabel)->block_id;
    landexp->falselabel=falselabel;
    landexp->codeIR();
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    if(landexp->attribute.T.type!=Type::BOOL)
        IRgenTypeConversetobool(bl,landexp->attribute.T.type);
    IRgenBrCond(bl, nowreg, landexp->truelabel, landexp->falselabel);
    nowlabel=landexp->truelabel;
    eqexp->truelabel=truelabel;
    eqexp->falselabel=falselabel;
    eqexp->codeIR();
    bl=llvmIR.GetBlock(nowfunc, nowlabel);
    if(eqexp->attribute.T.type!=Type::BOOL)
        IRgenTypeConversetobool(bl,eqexp->attribute.T.type);
    //IRgenBrCond(bl, nowreg, landexp->truelabel, landexp->falselabel);
}

// short circuit ||
void LOrExp_or::codeIR() { 
    //TODO("LOrExpOr CodeIR"); 2024.11.21
    lorexp->truelabel=truelabel;
    lorexp->falselabel=llvmIR.NewBlock(nowfunc, ++totlabel)->block_id;
    lorexp->codeIR();
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    if(lorexp->attribute.T.type!=Type::BOOL)
        IRgenTypeConversetobool(bl,lorexp->attribute.T.type);
    IRgenBrCond(bl, nowreg, lorexp->truelabel, lorexp->falselabel);
    nowlabel=lorexp->falselabel;
    landexp->truelabel=truelabel;
    landexp->falselabel=falselabel;
    landexp->codeIR();
    bl=llvmIR.GetBlock(nowfunc, nowlabel);
    if(landexp->attribute.T.type!=Type::BOOL)
        IRgenTypeConversetobool(bl,landexp->attribute.T.type);
}

void ConstExp::codeIR() { addexp->codeIR(); }

void Lval::codeIR() { 
    //TODO("Lval CodeIR"); 2024.11.22
    LLVMBlock bl= llvmIR.GetBlock(nowfunc, nowlabel);
    VarAttribute attr;
    int reg = irgen_table.symbol_table.lookup(name);
    if ( reg >= 0) {
        ptr = GetNewRegOperand(reg);
        attr = irgen_table.reg_table[reg];
    }
    else {
        ptr = GetNewGlobalOperand(name->get_string());
        attr = semant_table.GlobalTable[name];
    }
    std::vector<Operand> arr;
    if (dims != nullptr) {
        for (auto it = dims->begin(); it != dims->end(); ++it) {
    		auto dim = *it;
            dim->codeIR();
            if(dim->attribute.T.type==Type::BOOL){
                IRgenZextI1toI32(bl, nowreg, nowreg+1);
                nowreg++;
            }
            arr.push_back(GetNewRegOperand(nowreg));
        }
    }
    //std::cout<<name->get_string()<<' '<<attr.type<<'\n';
    BasicInstruction::LLVMType T =(attr.type ==Type::INT)?(BasicInstruction::I32):((attr.type==Type::FLOAT)?(BasicInstruction::FLOAT32):(BasicInstruction::PTR));
    //if(attribute.T.type==Type::PTR){
    if (!arr.empty() ||attribute.T.type==Type::PTR) {
        //T=BasicInstruction::PTR;
        if (attr.isfparam) {
            IRgenGetElementptrIndexI32(bl, T, ++nowreg, ptr,attr.dims, arr);
        } 
        else {
            arr.insert(arr.begin(), new ImmI32Operand(0));
            IRgenGetElementptrIndexI32(bl, T, ++nowreg, ptr,attr.dims, arr);
        }
        ptr= GetNewRegOperand(nowreg);
    }
    //std::cout<<name->get_string()<<' '<<bool(dims!=nullptr)<<' '<<(bool(dims!=nullptr)?dims->size():0)<<' '<<(attr.dims).size()<<'\n';
    //if(!isleft && ((dims!=nullptr) && dims->size() == (attr.dims).size()))
    if(!isleft && attribute.T.type!= Type::PTR)
        IRgenLoad(bl, T, ++nowreg, ptr);
}

void FuncRParams::codeIR() { 
    //TODO("FuncRParams CodeIR"); 2024.11.22
    /*LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    std::vector<std::pair<BasicInstruction::LLVMType, Operand>> args;
    auto fparams = semant_table.FunctionTable[fname]->formals;
    for (int i = 0; i < (*params).size(); ++i) {
        auto param = (*params)[i];
        auto fparam = (*fparams)[i];
        param->codeIR();
        BasicInstruction::LLVMType T =(fparam->attribute.T.type==Type::INT)?(BasicInstruction::I32):
                    ((fparam->attribute.T.type==Type::FLOAT)?(BasicInstruction::FLOAT32):(BasicInstruction::PTR));
        args.emplace_back(T, GetNewRegOperand(nowreg));
    }*/
   return;
}

//add 2024.11.22
//Symbol fname;
void Func_call::codeIR() { 
    //TODO("FunctionCall CodeIR"); 2024.11.22
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    Type::ty ret = semant_table.FunctionTable[name]->return_type;
    BasicInstruction::LLVMType T =(ret ==Type::INT)?(BasicInstruction::I32):((ret==Type::FLOAT)?(BasicInstruction::FLOAT32):(BasicInstruction::PTR));
    if(funcr_params == nullptr){
        if(ret==Type::VOID)
            IRgenCallVoidNoArgs(bl, BasicInstruction::VOID, name->get_string());
        else
            IRgenCallNoArgs(bl, T, ++nowreg, name->get_string());
    }
    else{
        //fname=name;
        //funcr_params->codeIR();
        //std::cout<<111;
        LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
        std::vector<std::pair<BasicInstruction::LLVMType, Operand> > args;
        //std::cout<<(*(funcr_params->get_params())).size();
        auto params = ((FuncRParams *)funcr_params)->params;
        //std::cout<<(*params).size();
        for (int i = 0; i < ((*params).size()); i++) {
            auto param = (*params)[i];
            auto fparam = (*(semant_table.FunctionTable[name]->formals))[i];
            param->codeIR();
            //std::cout<<i<<fparam->attribute.T.type<<'\n';
            BasicInstruction::LLVMType T =(fparam->attribute.T.type==Type::INT)?(BasicInstruction::I32):
                    ((fparam->attribute.T.type==Type::FLOAT)?(BasicInstruction::FLOAT32):(BasicInstruction::PTR));
            if(fparam->attribute.T.type==Type::INT){
                if(param->attribute.T.type==Type::FLOAT){
                    IRgenFptosi(bl, nowreg, nowreg+1);
                    nowreg++;
                }
                else if(param->attribute.T.type==Type::BOOL){
                    IRgenZextI1toI32(bl, nowreg, nowreg+1);
                    nowreg++;
                }
            }
            else if(fparam->attribute.T.type==Type::FLOAT){
                if(param->attribute.T.type==Type::INT){
                    IRgenSitofp(bl, nowreg, nowreg+1);
                    nowreg++;
                }
                else if(param->attribute.T.type==Type::BOOL){
                    IRgenZextI1toI32(bl, nowreg, nowreg+1);
                    IRgenSitofp(bl, nowreg+1, nowreg+2);
                    nowreg++;
                }
            }
            args.emplace_back(T, GetNewRegOperand(nowreg));
            //std::cout<<nowreg;
        }
        if (ret == Type::VOID) {
            IRgenCallVoid(bl,BasicInstruction::VOID, args, name->get_string());
        } else {
            IRgenCall(bl, T, ++nowreg, args, name->get_string());
        }
    }
}
    

void UnaryExp_plus::codeIR() { 
    //TODO("UnaryExpPlus CodeIR"); 2024.11.21
    unary_exp->codeIR();
}

void UnaryExp_neg::codeIR() { 
    //TODO("UnaryExpNeg CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    unary_exp->codeIR();
    if(unary_exp->attribute.T.type==Type::BOOL){
        IRgenZextI1toI32(bl, nowreg, nowreg+1);
        nowreg++;
    }
    if(unary_exp->attribute.T.type==Type::FLOAT)
        IRgenArithmeticF32ImmLeft(bl, BasicInstruction::FSUB, 0, nowreg, nowreg+1);
    else
        IRgenArithmeticI32ImmLeft(bl, BasicInstruction::SUB, 0, nowreg, nowreg+1);
    nowreg++;
}

void UnaryExp_not::codeIR() { 
    //TODO("UnaryExpNot CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    unary_exp->codeIR();
    if(unary_exp->attribute.T.type==Type::BOOL){
        IRgenZextI1toI32(bl, nowreg, nowreg+1);
        nowreg++;
    }
    if(unary_exp->attribute.T.type==Type::FLOAT)
        IRgenFcmpImmRight(bl, BasicInstruction::OEQ, nowreg, 0, nowreg+1);
    else
        IRgenIcmpImmRight(bl, BasicInstruction::eq, nowreg, 0, nowreg+1);
    nowreg++;
    IRgenZextI1toI32(bl, nowreg, nowreg+1);
    nowreg++;
}

void IntConst::codeIR() { 
    //TODO("IntConst CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    IRgenArithmeticI32ImmAll(bl, BasicInstruction::ADD, val, 0, ++nowreg);
}

void FloatConst::codeIR() { 
    //TODO("FloatConst CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    IRgenArithmeticF32ImmAll(bl, BasicInstruction::FADD, val, 0, ++nowreg);
}

void StringConst::codeIR() { 
    //TODO("StringConst CodeIR"); 2024.11.21
    return;
}

void PrimaryExp_branch::codeIR() { exp->codeIR(); }

void assign_stmt::codeIR() { 
    //TODO("AssignStmt CodeIR"); 2024.11.21
    lval->codeIR();
    exp->codeIR();
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    if(lval->attribute.T.type==Type::INT){
        if(exp->attribute.T.type==Type::FLOAT){
            IRgenFptosi(bl, nowreg, nowreg+1);
            nowreg++;
        }
        else if(exp->attribute.T.type==Type::BOOL){
            IRgenZextI1toI32(bl, nowreg, nowreg+1);
            nowreg++;
        }
        IRgenStore(bl,BasicInstruction::I32, nowreg, ((Lval *)lval)->ptr);
    }
    else if(lval->attribute.T.type==Type::FLOAT){
        if(exp->attribute.T.type==Type::INT){
            IRgenSitofp(bl, nowreg, nowreg+1);
            nowreg++;
        }
        else if(exp->attribute.T.type==Type::BOOL){
            IRgenZextI1toI32(bl, nowreg, nowreg+1);
            IRgenSitofp(bl, nowreg+1,nowreg+2);
            nowreg+=2;
        }
        IRgenStore(bl,BasicInstruction::FLOAT32, nowreg, ((Lval *)lval)->ptr);
    }
}

void expr_stmt::codeIR() { exp->codeIR(); }

void block_stmt::codeIR() { 
    //TODO("BlockStmt CodeIR"); 2024.11.21
    irgen_table.symbol_table.enter_scope();
    b->codeIR();
    irgen_table.symbol_table.exit_scope();
}

void ifelse_stmt::codeIR() { 
    //TODO("IfElseStmt CodeIR"); 2024.11.21
    Cond->truelabel=llvmIR.NewBlock(nowfunc, ++totlabel)->block_id;
    Cond->falselabel=llvmIR.NewBlock(nowfunc, ++totlabel)->block_id;
    int finlabel=llvmIR.NewBlock(nowfunc, ++totlabel)->block_id;
    Cond->codeIR();
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    if(Cond->attribute.T.type!=Type::BOOL)
        IRgenTypeConversetobool(bl, Cond->attribute.T.type);
    IRgenBrCond(bl, nowreg, Cond->truelabel, Cond->falselabel);
    nowlabel=Cond->truelabel;
    ifstmt->codeIR();
    bl=llvmIR.GetBlock(nowfunc, nowlabel);
    IRgenBRUnCond(bl,finlabel);
    nowlabel=Cond->falselabel;
    elsestmt->codeIR();
    bl=llvmIR.GetBlock(nowfunc, nowlabel);
    IRgenBRUnCond(bl,finlabel);
    nowlabel=finlabel;
}

void if_stmt::codeIR() { 
    //TODO("IfStmt CodeIR"); 2024.11.21
    Cond->truelabel=llvmIR.NewBlock(nowfunc, ++totlabel)->block_id;
    Cond->falselabel=llvmIR.NewBlock(nowfunc, ++totlabel)->block_id;
    Cond->codeIR();
    //std::cout<<Cond->truelabel<<' '<<Cond->falselabel;
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    if(Cond->attribute.T.type!=Type::BOOL)
        IRgenTypeConversetobool(bl, Cond->attribute.T.type);
    IRgenBrCond(bl, nowreg, Cond->truelabel, Cond->falselabel);
    nowlabel=Cond->truelabel;
    ifstmt->codeIR();
    bl=llvmIR.GetBlock(nowfunc, nowlabel);
    IRgenBRUnCond(bl,Cond->falselabel);
    nowlabel=Cond->falselabel;
}

void while_stmt::codeIR() { 
    //TODO("WhileStmt CodeIR"); 2024.11.21
    int condlabel=llvmIR.NewBlock(nowfunc, ++totlabel)->block_id;
    Cond->truelabel=llvmIR.NewBlock(nowfunc, ++totlabel)->block_id;
    Cond->falselabel=llvmIR.NewBlock(nowfunc, ++totlabel)->block_id;
    int fwhilelabel[2];fwhilelabel[0]=whilelabel[0];fwhilelabel[1]=whilelabel[1];
    whilelabel[0]=condlabel;whilelabel[1]=Cond->falselabel;
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    IRgenBRUnCond(bl, condlabel);
    nowlabel=condlabel;
    Cond->codeIR();
    bl = llvmIR.GetBlock(nowfunc, nowlabel);
    if(Cond->attribute.T.type!=Type::BOOL)
        IRgenTypeConversetobool(bl, Cond->attribute.T.type);
    IRgenBrCond(bl, nowreg, Cond->truelabel, Cond->falselabel);
    nowlabel=Cond->truelabel;
    body->codeIR();
    bl = llvmIR.GetBlock(nowfunc, nowlabel);
    IRgenBRUnCond(bl,condlabel);
    nowlabel=Cond->falselabel;
    whilelabel[0]=fwhilelabel[0];whilelabel[1]=fwhilelabel[1];
}

void continue_stmt::codeIR() { 
    //TODO("ContinueStmt CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    IRgenBRUnCond(bl, whilelabel[0]);
}

void break_stmt::codeIR() { 
    //TODO("BreakStmt CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    IRgenBRUnCond(bl, whilelabel[1]);
}

void return_stmt::codeIR() { 
    //TODO("ReturnStmt CodeIR"); 2024.11.21
    return_exp->codeIR();
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    if(func_ret==Type::INT){
        if(return_exp->attribute.T.type==Type::FLOAT){
            IRgenFptosi(bl, nowreg, nowreg+1);
            nowreg++;
        }
        else if(return_exp->attribute.T.type==Type::BOOL){
            IRgenZextI1toI32(bl, nowreg, nowreg+1);
            nowreg++;
        }
        IRgenRetReg(bl, BasicInstruction::I32, nowreg);
    }
    else if(func_ret==Type::FLOAT){
        if(return_exp->attribute.T.type==Type::INT){
            IRgenSitofp(bl, nowreg, nowreg+1);
            nowreg++;
        }
        else if(return_exp->attribute.T.type==Type::BOOL){
            IRgenZextI1toI32(bl, nowreg, nowreg+1);
            IRgenSitofp(bl, nowreg+1, nowreg+2);
            nowreg++;
            nowreg++;
        }
        IRgenRetReg(bl, BasicInstruction::FLOAT32, nowreg);
    }

}

void return_stmt_void::codeIR() { 
    //TODO("ReturnStmtVoid CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    IRgenRetVoid(bl);
}

void ConstInitVal::codeIR() { 
    //TODO("ConstInitVal CodeIR"); 2024.11.22
}

void ConstInitVal_exp::codeIR() { 
    //TODO("ConstInitValWithExp CodeIR"); 2024.11.22
    exp->codeIR();
}

void VarInitVal::codeIR() { 
    //TODO("VarInitVal CodeIR"); 2024.11.22
}

void VarInitVal_exp::codeIR() { 
    //TODO("VarInitValWithExp CodeIR"); 2024.11.22
    exp->codeIR();
}

void Arrayinit(LLVMBlock bl,BasicInstruction::LLVMType T,Operand ptr,InitVal init,std::vector<int> va,std::vector<Operand> arr){
    auto initval=init->get_initval();
    int pos=-1;
    for (auto it=initval->begin();it!=initval->end();it++){
        pos++;
        auto i=*it;
        auto exp=i->get_exp();
        if(exp){
            int size=va.size()-arr.size()+1;
            int nowpos=pos;
            std::vector <int> idx;
            std::vector<Operand> nowarr;
            nowarr.assign(arr.begin(),arr.end());
            for(int t=va.size()-1;t>=(arr.size()-1);t--){
                //std::cout<<nowpos<<nowpos%va[t];
                idx.push_back(nowpos%va[t]);
                nowpos/=va[t];
            }
            while(!idx.empty()){
                int idxn=idx.back();
                idx.pop_back();
                //std::cout<<"idxn"<<idxn;
                IRgenArithmeticI32ImmAll(bl, BasicInstruction::ADD,idxn,0,++nowreg);
                nowarr.push_back(GetNewRegOperand(nowreg));
            }
            IRgenGetElementptrIndexI32(bl,T,++nowreg,ptr,va,nowarr);
            auto nowptr=GetNewRegOperand(nowreg);
            //if(exp->islval()) 
            //    ((Lval*)exp)-> 
            exp->codeIR();
            if(T==BasicInstruction::I32){
                if(exp->attribute.T.type==Type::FLOAT){
                    IRgenFptosi(bl, nowreg, nowreg+1);
                    nowreg++;
                }
                else if(exp->attribute.T.type==Type::BOOL){
                    IRgenZextI1toI32(bl, nowreg, nowreg+1);
                    nowreg++;
                }
            }
            else if(T==BasicInstruction::FLOAT32){
                if(exp->attribute.T.type==Type::INT){
                    IRgenSitofp(bl, nowreg, nowreg+1);
                    nowreg++;
                }
                else if(exp->attribute.T.type==Type::BOOL){
                    IRgenZextI1toI32(bl, nowreg, nowreg+1);
                    IRgenSitofp(bl, nowreg+1, nowreg+2);
                    nowreg++;
                    nowreg++;
                }
            }
            IRgenStore(bl,T, nowreg, nowptr);
        }
        else{
            int idxn=pos;
            std::vector<Operand> nowarr;
            nowarr.assign(arr.begin(),arr.end());
            IRgenArithmeticI32ImmAll(bl, BasicInstruction::ADD,idxn,0,++nowreg);
            nowarr.push_back(GetNewRegOperand(nowreg));
            Arrayinit(bl,T,ptr,i,va,nowarr);
        }
    }
}
/*
//ref后进行本土化，目标代码时进行的修改
void ArrayInitIR(LLVMBlock bl, const std::vector<int> dims, int arrayaddr_reg_no, InitVal init,
                          int beginPos, int endPos, int dimsIdx, Type::ty ArrayType) {
    int pos = beginPos;
    for (InitVal iv : *(init->get_initval())) {
        if (iv->get_exp()) {
            iv->codeIR();
            //IRgenTypeConverse(block, iv->attribute.T.type, ArrayType, init_val_reg);
            if(ArrayType==Type::INT){
                if(iv->attribute.T.type==Type::FLOAT){
                    IRgenFptosi(bl, nowreg, nowreg+1);
                    nowreg++;
                }
                else if(iv->attribute.T.type==Type::BOOL){
                    IRgenZextI1toI32(bl, nowreg, nowreg+1);
                    nowreg++;
                }
            }
            else if(ArrayType==Type::FLOAT){
                if(iv->attribute.T.type==Type::INT){
                    IRgenSitofp(bl, nowreg, nowreg+1);
                    nowreg++;
                }
                else if(iv->attribute.T.type==Type::BOOL){
                    IRgenZextI1toI32(bl, nowreg, nowreg+1);
                    IRgenSitofp(bl, nowreg+1, nowreg+2);
                    nowreg++;
                    nowreg++;
                }
            }
            int init_val_reg = nowreg;
            int addr_reg = nowreg;
            auto gep = new GetElementptrInstruction(Type2LLvm[ArrayType], GetNewRegOperand(addr_reg),
                                                    GetNewRegOperand(arrayaddr_reg_no), dims);
            gep->push_idx_imm32(0);
            std::vector<int> indexes = GetIndexes(dims, pos);
            for (int idx : indexes) {
                gep->push_idx_imm32(idx);
            }
            // %addr_reg = getelementptr i32, ptr %arrayaddr_reg_no, i32 0, i32 ...
            bl->InsertInstruction(1, gep);
            IRgenStore(bl, Type2LLvm[ArrayType], GetNewRegOperand(init_val_reg), GetNewRegOperand(addr_reg));
            pos++;
        }
        else {
            int max_subBlock_sz = 0;
            int min_dim_step = FindMinDimStepIR(dims, pos - beginPos, dimsIdx, max_subBlock_sz);
            ArrayInitIR(bl, dims, arrayaddr_reg_no, iv, pos, pos + max_subBlock_sz - 1,
                                 dimsIdx + min_dim_step, ArrayType);
            pos += max_subBlock_sz;
        }
    }
}
*/
void VarDef_no_init::codeIR() { 
    //TODO("VarDefNoInit CodeIR"); 2024.11.22
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    irgen_table.symbol_table.add_Symbol(name, ++nowreg);
    int varreg=nowreg;
    VarAttribute va;
    va.type = type;
    auto T=(type ==Type::INT)?(BasicInstruction::I32):(BasicInstruction::FLOAT32);
    if (dims == nullptr) { 
        IRgenAlloca(bl, T, nowreg);
        irgen_table.reg_table[nowreg] = va;
        Operand op;
        if (type == Type::INT) 
            IRgenArithmeticI32ImmAll(bl, BasicInstruction::ADD, 0, 0, ++nowreg);
        else
            IRgenArithmeticF32ImmAll(bl, BasicInstruction::FADD, 0, 0, ++nowreg);
        op= GetNewRegOperand(nowreg);
        IRgenStore(bl,T,op, GetNewRegOperand(varreg));
    }
    else{
        for(auto it=dims->begin();it!=dims->end();it++)
            va.dims.push_back((*it)->attribute.V.val.IntVal);
        irgen_table.reg_table[nowreg] = va;
        IRgenAllocaArray(bl,T,nowreg,va.dims);
    }
}

void VarDef::codeIR() { 
    //TODO("VarDef CodeIR"); 2024.11.22
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    irgen_table.symbol_table.add_Symbol(name, ++nowreg);
    int varreg=nowreg;
    VarAttribute va;
    va.type = type;
    auto T=(type ==Type::INT)?(BasicInstruction::I32):(BasicInstruction::FLOAT32);
    if (dims == nullptr) { 
        IRgenAlloca(bl, T, nowreg);
        irgen_table.reg_table[nowreg] = va;
        Operand op;
        Expression initExp = init->get_exp();
        initExp->codeIR();
        if(type==Type::INT){
            if(initExp->attribute.T.type==Type::FLOAT){
                IRgenFptosi(bl, nowreg, nowreg+1);
                nowreg++;
            }
            else if(initExp->attribute.T.type==Type::BOOL){
                IRgenZextI1toI32(bl, nowreg, nowreg+1);
                nowreg++;
            }
        }
        else if(type==Type::FLOAT){
            if(initExp->attribute.T.type==Type::INT){
                IRgenSitofp(bl, nowreg, nowreg+1);
                nowreg++;
            }
            else if(initExp->attribute.T.type==Type::BOOL){
                IRgenZextI1toI32(bl, nowreg, nowreg+1);
                IRgenSitofp(bl, nowreg+1, nowreg+2);
                nowreg++;
            }
        }
        op = GetNewRegOperand(nowreg);
        IRgenStore(bl,T,op, GetNewRegOperand(varreg));
    }
    else{
        for(auto it=dims->begin();it!=dims->end();it++)
            va.dims.push_back((*it)->attribute.V.val.IntVal);
        irgen_table.reg_table[nowreg] = va;
        Operand op = GetNewRegOperand(nowreg);
        IRgenAllocaArray(bl,T,nowreg,va.dims);
        int sz = 1;
        for (int i=0;i<(va.dims).size();i++) sz*= va.dims[i];
        LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
        std::vector<std::pair<BasicInstruction::LLVMType, Operand>> args;
        args.emplace_back(BasicInstruction::PTR,GetNewRegOperand(nowreg));
        args.emplace_back(BasicInstruction::I8, new ImmI32Operand(0));
        args.emplace_back(BasicInstruction::I32, new ImmI32Operand(4*sz));
        args.emplace_back(BasicInstruction::I1, new ImmI32Operand(0));
        IRgenCallVoid(bl,BasicInstruction::VOID, args, std::string("llvm.memset.p0.i32"));
        std::vector<Operand> arr;
        arr.insert(arr.begin(), new ImmI32Operand(0));
        Arrayinit(bl,T,op,init,va.dims,arr);
        //ArrayInitIR(bl, va.dims, nowreg, init, 0, sz-1, 0, type);
    }
}

void ConstDef::codeIR() { 
    //TODO("ConstDef CodeIR"); 2024.11.22
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    irgen_table.symbol_table.add_Symbol(name, ++nowreg);
    int varreg=nowreg;
    VarAttribute va;
    va.type = type;
    auto T=(type ==Type::INT)?(BasicInstruction::I32):(BasicInstruction::FLOAT32);
    if (dims == nullptr) { 
        IRgenAlloca(bl, T, nowreg);
        irgen_table.reg_table[nowreg] = va;
        Operand op;
        Expression initExp = init->get_exp();
        initExp->codeIR();
        if(type==Type::INT){
            if(initExp->attribute.T.type==Type::FLOAT){
                IRgenFptosi(bl, nowreg, nowreg+1);
                nowreg++;
            }
            else if(initExp->attribute.T.type==Type::BOOL){
                IRgenZextI1toI32(bl, nowreg, nowreg+1);
                nowreg++;
            }
        }
        else if(type==Type::FLOAT){
            if(initExp->attribute.T.type==Type::INT){
                IRgenSitofp(bl, nowreg, nowreg+1);
                nowreg++;
            }
            else if(initExp->attribute.T.type==Type::BOOL){
                IRgenZextI1toI32(bl, nowreg, nowreg+1);
                IRgenSitofp(bl, nowreg+1, nowreg+2);
                nowreg++;
            }
        }
        op = GetNewRegOperand(nowreg);
        IRgenStore(bl,T,op, GetNewRegOperand(varreg));
    }
    else{
        for(auto it=dims->begin();it!=dims->end();it++)
            va.dims.push_back((*it)->attribute.V.val.IntVal);
        irgen_table.reg_table[nowreg] = va;
        Operand op = GetNewRegOperand(nowreg);
        IRgenAllocaArray(bl,T,nowreg,va.dims);
        int sz = 1;
        for (int i=0;i<(va.dims).size();i++) sz*= va.dims[i];
        LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
        std::vector<std::pair<BasicInstruction::LLVMType, Operand>> args;
        args.emplace_back(BasicInstruction::PTR,GetNewRegOperand(nowreg));
        args.emplace_back(BasicInstruction::I8, new ImmI32Operand(0));
        args.emplace_back(BasicInstruction::I32, new ImmI32Operand(4*sz));
        args.emplace_back(BasicInstruction::I1, new ImmI32Operand(0));
        IRgenCallVoid(bl,BasicInstruction::VOID, args, std::string("llvm.memset.p0.i32"));
        std::vector<Operand> arr;
        arr.insert(arr.begin(), new ImmI32Operand(0));
        Arrayinit(bl,T,op,init,va.dims,arr);
    }
}

void VarDecl::codeIR() { 
    //TODO("VarDecl CodeIR"); 2024.11.22
    for (auto it=var_def_list->begin();it!=var_def_list->end();it++) {
        auto vardef=*it;
        vardef->set_type(type_decl);
        vardef->codeIR();
    }
}

void ConstDecl::codeIR() { 
    //TODO("ConstDecl CodeIR"); 2024.11.22
    for (auto it=var_def_list->begin();it!=var_def_list->end();it++) {
        //std::cout<<1111;
        auto constdef=*it;
        constdef->set_type(type_decl);
        constdef->codeIR();
    }
}

void BlockItem_Decl::codeIR() { 
    //TODO("BlockItemDecl CodeIR"); 2024.11.22
    decl->codeIR();
}

void BlockItem_Stmt::codeIR() { 
    //TODO("BlockItemStmt CodeIR"); 2024.11.22
    stmt->codeIR();
}

void __Block::codeIR() { 
    //TODO("Block CodeIR"); 2024.11.21
    irgen_table.symbol_table.enter_scope();
    for (auto it = item_list->begin(); it != item_list->end(); ++it) 
        (*it)->codeIR();
    irgen_table.symbol_table.exit_scope();
}

int cntfparam=-1;
int regalloc=0;

void __FuncFParam::codeIR() { 
    //TODO("FunctionFParam CodeIR"); 2024.11.21
    LLVMBlock bl = llvmIR.GetBlock(nowfunc, nowlabel);
    VarAttribute va;
    va.type=type_decl;
    BasicInstruction::LLVMType T =(type_decl ==Type::INT)?(BasicInstruction::I32):(BasicInstruction::FLOAT32);
    if(dims == nullptr){
        regalloc++;
        nowfunc->InsertFormal(T);
        nowreg++;
        IRgenAlloca(bl,T, cntfparam+regalloc);
        IRgenStore(bl,T, GetNewRegOperand(nowreg), GetNewRegOperand(cntfparam+regalloc));
        irgen_table.symbol_table.add_Symbol(name, cntfparam+regalloc);
        irgen_table.reg_table[cntfparam+regalloc] = va;
    }
    else{
        //std::cout<<"ARRAYPARAM";
        nowfunc->InsertFormal(BasicInstruction::PTR);
        for (auto it = ++(dims->begin()); it != dims->end(); ++it){
            va.dims.push_back((*it)->attribute.V.val.IntVal);
        }
        irgen_table.symbol_table.add_Symbol(name, ++nowreg);
        va.isfparam=true;
        irgen_table.reg_table[nowreg] = va;
    }
}

void __FuncDef::codeIR() { 
    //TODO("FunctionDef CodeIR"); 2024.11.21
    //std::cout<<"def";
    irgen_table.symbol_table.enter_scope();
    func_ret=return_type;
    nowreg=-1;
    nowlabel=0;
    totlabel=-1;
    irgen_table.reg_table.clear();
    BasicInstruction::LLVMType T = (return_type ==Type::INT)?(BasicInstruction::I32):((return_type ==Type::FLOAT)?(BasicInstruction::FLOAT32):((BasicInstruction::VOID)));
    nowfunc = new FunctionDefineInstruction(T, name->get_string());
    llvmIR.NewFunction(nowfunc);
    LLVMBlock bl=llvmIR.NewBlock(nowfunc,++totlabel);
    cntfparam=(*formals).size();
    regalloc=0;
    //std::cout<<"def";
    for(auto it=formals->begin();it!=formals->end();it++)
        (*it)->codeIR();
    nowreg+=(regalloc+1);
    IRgenBRUnCond(bl,totlabel+1);
    bl = llvmIR.NewBlock(nowfunc, ++totlabel);
    nowlabel=totlabel;
    block->codeIR();
    irgen_table.symbol_table.exit_scope();
    for(int i=0;i<=totlabel;i++){
        LLVMBlock bl=llvmIR.GetBlock(nowfunc,i);
        //if(bl->Instruction_list.empty()){
        if(!bl->retbr){
            if(func_ret==Type::INT)
                IRgenRetImmInt(bl,BasicInstruction::I32,0);
            else if(func_ret==Type::FLOAT)
                IRgenRetImmFloat(bl,BasicInstruction::FLOAT32, 0.0);
            else
                IRgenRetVoid(bl);
        }
    }
    //add 2024.12.14
    nowfunc->reg = nowreg;
    nowfunc->label = totlabel;
}


void CompUnit_Decl::codeIR() { 
//TODO("CompUnitDecl CodeIR"); 2024.11.22
    Type t1;
	t1.type=decl->get_type_decl();
    BasicInstruction::LLVMType T = (t1.type ==Type::INT)?(BasicInstruction::I32):((t1.type ==Type::FLOAT)?(BasicInstruction::FLOAT32):((BasicInstruction::VOID)));

	for (auto it = (decl->get_var_def_list())->begin(); it != (decl->get_var_def_list())->end(); ++it) {
    	auto vdef = *it;
    	Instruction globalDecl;
        auto va = semant_table.GlobalTable[vdef->get_name()];

        if (vdef->dimnum.size()) 
            globalDecl = new GlobalVarDefineInstruction(vdef->get_name()->get_string(), T, va);
        else {
            if (vdef->get_init() == nullptr) 
                globalDecl = new GlobalVarDefineInstruction(vdef->get_name()->get_string(),T, nullptr);
            else if (T == BasicInstruction::I32) 
                globalDecl =new GlobalVarDefineInstruction(vdef->get_name()->get_string(), T, new ImmI32Operand(va.IntInitVals[0]));
            else if (T == BasicInstruction::FLOAT32) 
                globalDecl = new GlobalVarDefineInstruction(vdef->get_name()->get_string(), T,new ImmF32Operand(va.FloatInitVals[0]));
        }
        llvmIR.global_def.push_back(globalDecl);
    }
}

void CompUnit_FuncDef::codeIR() { func_def->codeIR(); }

void AddLibFunctionDeclare() {
    FunctionDeclareInstruction *getint = new FunctionDeclareInstruction(BasicInstruction::I32, "getint");
    llvmIR.function_declare.push_back(getint);

    FunctionDeclareInstruction *getchar = new FunctionDeclareInstruction(BasicInstruction::I32, "getch");
    llvmIR.function_declare.push_back(getchar);

    FunctionDeclareInstruction *getfloat = new FunctionDeclareInstruction(BasicInstruction::FLOAT32, "getfloat");
    llvmIR.function_declare.push_back(getfloat);

    FunctionDeclareInstruction *getarray = new FunctionDeclareInstruction(BasicInstruction::I32, "getarray");
    getarray->InsertFormal(BasicInstruction::PTR);
    llvmIR.function_declare.push_back(getarray);

    FunctionDeclareInstruction *getfloatarray = new FunctionDeclareInstruction(BasicInstruction::I32, "getfarray");
    getfloatarray->InsertFormal(BasicInstruction::PTR);
    llvmIR.function_declare.push_back(getfloatarray);

    FunctionDeclareInstruction *putint = new FunctionDeclareInstruction(BasicInstruction::VOID, "putint");
    putint->InsertFormal(BasicInstruction::I32);
    llvmIR.function_declare.push_back(putint);

    FunctionDeclareInstruction *putch = new FunctionDeclareInstruction(BasicInstruction::VOID, "putch");
    putch->InsertFormal(BasicInstruction::I32);
    llvmIR.function_declare.push_back(putch);

    FunctionDeclareInstruction *putfloat = new FunctionDeclareInstruction(BasicInstruction::VOID, "putfloat");
    putfloat->InsertFormal(BasicInstruction::FLOAT32);
    llvmIR.function_declare.push_back(putfloat);

    FunctionDeclareInstruction *putarray = new FunctionDeclareInstruction(BasicInstruction::VOID, "putarray");
    putarray->InsertFormal(BasicInstruction::I32);
    putarray->InsertFormal(BasicInstruction::PTR);
    llvmIR.function_declare.push_back(putarray);

    FunctionDeclareInstruction *putfarray = new FunctionDeclareInstruction(BasicInstruction::VOID, "putfarray");
    putfarray->InsertFormal(BasicInstruction::I32);
    putfarray->InsertFormal(BasicInstruction::PTR);
    llvmIR.function_declare.push_back(putfarray);

    FunctionDeclareInstruction *starttime = new FunctionDeclareInstruction(BasicInstruction::VOID, "_sysy_starttime");
    starttime->InsertFormal(BasicInstruction::I32);
    llvmIR.function_declare.push_back(starttime);

    FunctionDeclareInstruction *stoptime = new FunctionDeclareInstruction(BasicInstruction::VOID, "_sysy_stoptime");
    stoptime->InsertFormal(BasicInstruction::I32);
    llvmIR.function_declare.push_back(stoptime);

    // 一些llvm自带的函数，也许会为你的优化提供帮助
    FunctionDeclareInstruction *llvm_memset =
    new FunctionDeclareInstruction(BasicInstruction::VOID, "llvm.memset.p0.i32");
    llvm_memset->InsertFormal(BasicInstruction::PTR);
    llvm_memset->InsertFormal(BasicInstruction::I8);
    llvm_memset->InsertFormal(BasicInstruction::I32);
    llvm_memset->InsertFormal(BasicInstruction::I1);
    llvmIR.function_declare.push_back(llvm_memset);

    FunctionDeclareInstruction *llvm_umax = new FunctionDeclareInstruction(BasicInstruction::I32, "llvm.umax.i32");
    llvm_umax->InsertFormal(BasicInstruction::I32);
    llvm_umax->InsertFormal(BasicInstruction::I32);
    llvmIR.function_declare.push_back(llvm_umax);

    FunctionDeclareInstruction *llvm_umin = new FunctionDeclareInstruction(BasicInstruction::I32, "llvm.umin.i32");
    llvm_umin->InsertFormal(BasicInstruction::I32);
    llvm_umin->InsertFormal(BasicInstruction::I32);
    llvmIR.function_declare.push_back(llvm_umin);

    FunctionDeclareInstruction *llvm_smax = new FunctionDeclareInstruction(BasicInstruction::I32, "llvm.smax.i32");
    llvm_smax->InsertFormal(BasicInstruction::I32);
    llvm_smax->InsertFormal(BasicInstruction::I32);
    llvmIR.function_declare.push_back(llvm_smax);

    FunctionDeclareInstruction *llvm_smin = new FunctionDeclareInstruction(BasicInstruction::I32, "llvm.smin.i32");
    llvm_smin->InsertFormal(BasicInstruction::I32);
    llvm_smin->InsertFormal(BasicInstruction::I32);
    llvmIR.function_declare.push_back(llvm_smin);
}
