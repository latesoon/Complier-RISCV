#include "../../include/Instruction.h"
#include "../../include/ir.h"
#include <assert.h>

void LLVMIR::CFGInit() {
    //add 2024.12.21
    llvm_cfg.clear();
    for (auto &[defI, bb_map] : function_block_map) {
        CFG *cfg = new CFG();
        cfg->block_map = &bb_map;
        cfg->function_def = defI;

        //add 2024.12.14
        cfg->reg = defI->reg;
        cfg->label = defI->label;
        
        cfg->BuildCFG();
        //TODO("init your members in class CFG if you need");
        llvm_cfg[defI] = cfg;
    }
}

void LLVMIR::BuildCFG() {
    for (auto [defI, cfg] : llvm_cfg) {
        cfg->BuildCFG();
    }
}

void CFG::BuildCFG() { 
    //TODO("BuildCFG"); 2024.12.14
    G.resize(label + 1);
    invG.resize(label + 1);//改变map大小
    std::queue<int> q;
    std::set<int> reach;
    q.push(0);
    reach.emplace(0);
    while(!q.empty()){
        int now = q.front();
        q.pop();
        auto& bl = (*block_map)[now];
        auto& ins = bl->Instruction_list;
        int retbr;
        for (int i = 0; i < ins.size(); i++) {
            auto nowins = ins[i];
            if (nowins->GetOpcode() == BasicInstruction::RET){
                retbr = i;
                break;
            }
            else if (nowins->GetOpcode() == BasicInstruction::BR_COND){
                retbr = i;
                BrCondInstruction *cond = (BrCondInstruction *)nowins;
                int truel = ((LabelOperand *)cond->GetTrueLabel())->GetLabelNo();
                G[now].push_back((*block_map)[truel]);
                invG[truel].push_back(bl);
                if (reach.find(truel) == reach.end()){
                    q.push(truel);
                    reach.emplace(truel);
                }
                int falsel = ((LabelOperand *)cond->GetFalseLabel())->GetLabelNo();
                G[now].push_back((*block_map)[falsel]);
                invG[falsel].push_back(bl);
                if (reach.find(falsel) == reach.end()){
                    q.push(falsel);
                    reach.emplace(falsel);
                }
                break;
            }
            else if (nowins->GetOpcode() == BasicInstruction::BR_UNCOND){
                retbr = i;
                BrUncondInstruction *cond = (BrUncondInstruction *)nowins;
                int tol = ((LabelOperand *)cond->GetDestLabel())->GetLabelNo();
                G[now].push_back((*block_map)[tol]);
                invG[tol].push_back(bl);
                if (reach.find(tol) == reach.end()){
                    q.push(tol);
                    reach.emplace(tol);
                }
                break;
            }
        }
        while(ins.size() > retbr + 1) ins.pop_back();//delete useless code
    }
    for(int i = 0;i <= label; i++){//delete useless block
        if(((*block_map).find(i)!=(*block_map).end()) && (reach.find(i) == reach.end()))
            (*block_map).erase(i);
    }
    if(!reach.empty()){
        label=*reach.rbegin();
    }
}

std::vector<LLVMBlock> CFG::GetPredecessor(LLVMBlock B) { return invG[B->block_id]; }

std::vector<LLVMBlock> CFG::GetPredecessor(int bbid) { return invG[bbid]; }

std::vector<LLVMBlock> CFG::GetSuccessor(LLVMBlock B) { return G[B->block_id]; }

std::vector<LLVMBlock> CFG::GetSuccessor(int bbid) { return G[bbid]; }