//2024.12.20 23:08
#ifndef TCO_H
#define TCO_H
#include "../../include/ir.h"
#include "../pass.h"

#include "../analysis/def_use.h"

class TcoPass : public IRPass {
private:
    DefUseAnalysis *defuse;
    void Tco(FuncDefInstruction defI,CFG *C);

public:
    TcoPass(LLVMIR *IR) : IRPass(IR) {}
    void Execute();
};

#endif