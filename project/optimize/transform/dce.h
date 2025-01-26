//2024.12.20 1:33
#ifndef DCE_H
#define DCE_H
#include "../../include/ir.h"
#include "../pass.h"

#include "../analysis/def_use.h"

class DcePass : public IRPass {
private:
    DefUseAnalysis *defuse;
    void Dce(CFG *C);

public:
    DcePass(LLVMIR *IR, DefUseAnalysis *du) : IRPass(IR) { defuse = du; }
    void Execute();
};

#endif