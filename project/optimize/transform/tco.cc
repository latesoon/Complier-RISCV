//2024.12.20 23:08
#include "tco.h"

void TcoPass::Tco(FuncDefInstruction defI, CFG *C){
    bool need_tco = false;
    auto& b1=(*C->block_map)[1];
    std::vector<PhiInstruction*> insphi{};
    for(auto [id,b] : *C->block_map){
        auto& list=b->Instruction_list;
        if((list.size()>=2) && ((*(list.rbegin()))->GetOpcode() == BasicInstruction::RET) && ((*(list.rbegin()+1))->GetOpcode() == BasicInstruction::CALL)){
            auto call = (CallInstruction*)(*(list.rbegin()+1));
            if (call->GetFunctionName() != defI->GetFunctionName()) continue;
            if(!need_tco){
                need_tco = true;//1st do,need init
                auto paralist = call->GetParameterList();
                auto deflist = defI->formals_reg;
                int idx = 0;
                for(auto para : paralist){
                    auto op = GetNewRegOperand(++C->reg);
                    PhiInstruction* phi = new PhiInstruction(para.first,op);
                    (phi->phi_list).emplace_back(GetNewLabelOperand(0),deflist[idx++]);
                    (phi->phi_list).emplace_back(GetNewLabelOperand(id),para.second);
                    insphi.push_back(phi);
                    b1->InsertInstruction(0,phi);
                }
            }
            else{
                auto paralist = call->GetParameterList();
                int idx = 0;
                for(auto para : paralist)
                    (insphi[idx++]->phi_list).emplace_back(GetNewLabelOperand(id),para.second);
            }
            list.pop_back();
            list.pop_back();
            auto br = new BrUncondInstruction(GetNewLabelOperand(1));
            b->InsertInstruction(1,br);
        }
    }
    if(!need_tco) return;
    for(auto [id,b] : *C->block_map){
        for(auto& i : b->Instruction_list){
            auto use =i->getuseno();
            if(use.empty()) continue;
            for(int r : use){
                if(r < insphi.size())
                    i->updateuse(r,insphi[r]->result);
            }
        }
    }
}

void TcoPass::Execute() {
    for (auto [defI, cfg] : llvmIR->llvm_cfg) {
        Tco(defI, cfg);
    }
}