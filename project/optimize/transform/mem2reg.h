#ifndef MEM2REG_H
#define MEM2REG_H
#include "../../include/ir.h"
#include "../pass.h"

#include "../analysis/dominator_tree.h"

//add 2024.12.16
struct allocop{
    std::set<int> load{};
    std::set<int> store{};
};

class Mem2RegPass : public IRPass {
private:
    DomAnalysis *domtrees;
    // TODO():添加更多你需要的成员变量
    bool IsPromotable(CFG *C, Instruction AllocaInst);
    void Mem2RegNoUseAlloca(CFG *C, std::set<int> &vset);
    void Mem2RegUseDefInSameBlock(CFG *C, std::set<int> &vset, int block_id);
    void Mem2RegOneDefDomAllUses(CFG *C, std::set<int> &vset);
    void InsertPhi(CFG *C);
    void VarRename(CFG *C);
    void Mem2Reg(CFG *C);

    //add 2024.12.16
    std::map<int,allocop> alloc;
    std::map<int, std::pair<BasicInstruction::LLVMType,allocop> > phi;
    std::map<int,RegOperand *> latest;
    void Allocconstruct(CFG* C);

    //2024.12.19
    std::vector<std::map<int,PhiInstruction *> > phimap;
    std::vector<std::map<int,RegOperand *> > latestmap;

public:
    Mem2RegPass(LLVMIR *IR, DomAnalysis *dom) : IRPass(IR) { domtrees = dom; }
    void Execute();
};

#endif