//2024.12.20 1:33
#include "dce.h"

void DcePass::Dce(CFG *C){
    bool update = true;
    std::set<int> to_del;
    auto& usemap = defuse->IRUse[C];
    /*
    for(auto [id,m]:usemap){
        std::cout<<id<<':';
        for(auto i:m){
            std::cout<<i<<' ';
        }
        std::cout<<'\n';
    }
    */
    while(update){
        update = false;
        for(auto [id,b] : *C->block_map){
            for(auto i : b->Instruction_list){
                if(i->GetOpcode()==BasicInstruction::CALL) continue;
                int ret = i->getretno();
                if((ret != -1) && ((usemap.find(ret) == usemap.end()) || (usemap[ret].size() == 0)) && (to_del.find(ret) == to_del.end())){
                    //ret and no use
                    to_del.insert(ret);
                    update = true;
                    auto v = i->getuseno();
                    for(int r : v){
                        if(usemap[r].find(ret)!=usemap[r].end())
                            usemap[r].erase(usemap[r].find(ret));
                    }
                    //std::cout<<111;
                }
            }
        }
    }
    for(auto [id,b] : *C->block_map){
        std::deque<Instruction> newIns{};
        for(auto i : b->Instruction_list){
            int ret = i->getretno();
            if(to_del.find(ret) != to_del.end())
                continue;
            newIns.push_back(i);
        }
        b->Instruction_list = newIns;
    }
}

void DcePass::Execute() {
    for (auto [defI, cfg] : llvmIR->llvm_cfg) {
        Dce(cfg);
    }
}