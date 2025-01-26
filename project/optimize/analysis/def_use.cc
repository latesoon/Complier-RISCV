#include "def_use.h"
#include "../../include/ir.h"

/*
伪代码:
for BasicBlock bb in C
    for Instruction I in bb
        if I 有结果寄存器
            int regno = I->GetResultRegNo()
            IRDefMaps[C][regno] = I
*/
void DefUseAnalysis::GetDefUseInSingleCFG(CFG* C) {
    //TODO("GetDefUseInSingleCFG"); 2024.12.19
    auto &defmap = IRDefMaps[C];
    auto &usemap = IRUse[C];
    for(auto [id,b]: *C->block_map){
        for (auto i: b->Instruction_list){
            int ret = i->getretno();
            if(ret != -1)
                defmap[ret] = i;
            auto use = i->getuseno();
            for(int u : use)
                usemap[u].insert(ret);//不需要对无返回值进行特判,因为暂时删不掉,just insert -1 ok
        }
    }
    /*
    for(auto [id,m]:usemap){
        std::cout<<id<<':';
        for(auto i:m){
            std::cout<<i<<' ';
        }
        std::cout<<'\n';
    }
    */
}

void DefUseAnalysis::Execute() {

    //add 2024.12.20
    IRDefMaps.clear();
    IRUse.clear();
    
    for (auto [defI, cfg] : llvmIR->llvm_cfg) {
        GetDefUseInSingleCFG(cfg);
    }
}