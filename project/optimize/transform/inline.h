//2024.12.20 10:59
#ifndef INLINE_H
#define INLINE_H
#include "../../include/ir.h"
#include "../pass.h"
#include "dce.h"

struct detail{
    int insnum = 0;
    bool iscall = false;
    int becalled = 0;
};

class InlinePass : public IRPass {
private:
    DcePass* dce;
    bool Inline(FuncDefInstruction defI,CFG *C);
    void FuncInit();
    CFG* CFGCopy(CFG* C);
    std::map<std::string,detail> funcdetail;
public:
    InlinePass(LLVMIR *IR,DcePass* pass) : IRPass(IR) {dce=pass;}
    void Execute();
};

#endif