//2024.12.20 10:59
#include "inline.h"

const int maxinlinelen = 25;
const int maxtotlen = 200;
//当原func与要展开的func长度之和小于200,一定展开,否则仅展开长度小于25且没有自定义函数调用的函数

CFG* InlinePass::CFGCopy(CFG *C) {
    CFG *newCFG = new CFG;
    newCFG->label = C->label;
    newCFG->function_def = new FunctionDefineInstruction((C->function_def)->GetReturnType(),(C->function_def)->GetFunctionName());
    newCFG->block_map = new std::map<int, LLVMBlock>;
    for (auto [id, bb] : *C->block_map) {
        LLVMBlock newblock=new BasicBlock(id);
        (*newCFG->block_map)[id]= newblock;
        for (auto I : bb->Instruction_list) {
            auto newI = I->CopyInst();
            newblock->InsertInstruction(1, newI);
        }
    }
    return newCFG;
}

bool InlinePass::Inline(FuncDefInstruction defI,CFG *C){
    auto nowfunc = defI->GetFunctionName();
    bool isupdate = false,update = true;
    while(update){
        update = false;
        Instruction targetI;
        LLVMBlock&& targetblock=nullptr;
        for(auto [id,b]: *C->block_map){
            targetI = nullptr;
            targetblock = nullptr;
            for(auto i :b->Instruction_list){
                if(i->GetOpcode()==BasicInstruction::CALL){
                    auto name = ((CallInstruction*)i)->GetFunctionName();
                    if(funcdetail.find(name) != funcdetail.end()){
                        if(((funcdetail[nowfunc].insnum + funcdetail[name].insnum) <= maxtotlen) || ((!funcdetail[name].iscall) && (funcdetail[name].insnum <= maxinlinelen))){
                            targetI = i;
                            update = true;
                            isupdate = true;
                            break;
                        }
                    }
                }
            }
            if(targetI != nullptr){//find inline
                targetblock = b;
                //std::cout<<b->block_id;
                break;
            }
        }
        //if(targetblock!=nullptr)std::cout<<1;
        //std::cout<<(targetblock->block_id);
        //isupdate=false;
        //update=false;(for test)

        //if(!update) break;
        
        int nowlabel = C->label;
        int nowreg = C->reg;
        std::map<int,LLVMBlock> newblockmap{}; 

        auto new_block = new BasicBlock(++nowlabel);
        newblockmap[nowlabel]=new_block;
        std::deque<Instruction> origin_list;//update end
        std::deque<Instruction> new_list;
        bool aftertarget = false;
        for(auto i :targetblock->Instruction_list){
            /*if(i == targetI){
                aftertarget = true;
                continue;
            }*/
            if(aftertarget){
                new_list.push_back(i);
            }
            else{
                origin_list.push_back(i);
            }
        }/*
        new_block->Instruction_list = new_list;

        std::map<int,RegOperand*> newregmap{};

        int beginint = 0;
        auto regs = targetI->GetAllReg();
        RegOperand* returnreg = nullptr;
        PhiInstruction *phiins = nullptr;
        if(targetI->getretno() != -1){
            returnreg =regs.back();
            regs.pop_back();
            phiins = new PhiInstruction(((CallInstruction*)targetI)->GetRetType(), returnreg);
        }
        

        for(auto r :regs){
            newregmap[beginint++]= GetNewRegOperand(++nowreg);
        }

        CFG* inlineC = nullptr;
        for(auto [defI,C] : llvmIR->llvm_cfg){
            if(defI->GetFunctionName() == ((CallInstruction*)targetI)->GetFunctionName()){
                inlineC = CFGCopy(C);
                break;
            }
        }

        int endlabel = nowlabel;

        origin_list.push_back(new BrUncondInstruction(GetNewLabelOperand(endlabel+1)));
        targetblock->Instruction_list = origin_list;
        
        std::map <int,int> newlabelmap{};
        for(auto [id,b] : *inlineC->block_map){
            auto new_bl = new BasicBlock(++nowlabel);
            newblockmap[nowlabel] = new_bl;
            newlabelmap[id] = nowlabel;
        }
        for(auto [id,b] : *inlineC->block_map){
            std::deque<Instruction> new_li;
            for(auto i : b->Instruction_list){
                auto rlist =i->GetAllReg();
                for(auto r : rlist){
                    int regno = r->GetRegNo();
                    if(newregmap.find(regno)!=newregmap.end()){
                        r = newregmap[regno];
                    }
                    else{
                        newregmap[regno]=GetNewRegOperand(++nowreg);
                        r = newregmap[regno];
                    }
                }
                new_li.push_back(i);
            }
            auto back = new_li.back();
            if(back->GetOpcode() == BasicInstruction::RET){
                new_li.pop_back();
                new_li.push_back(new BrUncondInstruction(GetNewLabelOperand(endlabel)));
                if(returnreg != nullptr){
                    phiins->phi_list.emplace_back(GetNewLabelOperand(nowlabel),((RetInstruction*)back)->GetRetVal());
                }
            }
            else if(back->GetOpcode() == BasicInstruction::BR_COND){
                auto t_label= ((BrCondInstruction*)back)->GetTrueLabel();
                t_label = GetNewLabelOperand(newlabelmap[((LabelOperand*)t_label)->GetLabelNo()]);
                auto f_label= ((BrCondInstruction*)back)->GetFalseLabel();
                f_label = GetNewLabelOperand(newlabelmap[((LabelOperand*)f_label)->GetLabelNo()]);
            }
            else if(back->GetOpcode() == BasicInstruction::BR_UNCOND){
                auto d_label= ((BrUncondInstruction*)back)->GetDestLabel();
                d_label = GetNewLabelOperand(newlabelmap[((LabelOperand*)d_label)->GetLabelNo()]);
            }
            newblockmap[newlabelmap[id]]->Instruction_list=new_li;
        }
        
        if(returnreg != nullptr){
            int len=phiins->phi_list.size();
            if(len == 1){
                auto type=phiins->type;
                if(type == BasicInstruction::I32){
                    auto al= new ArithmeticInstruction(BasicInstruction::ADD, type, (phiins->phi_list)[0].second, new ImmI32Operand(0), phiins->result);
                    newblockmap[endlabel]->InsertInstruction(0,al);
                }
                else if(type ==BasicInstruction::FLOAT32){
                    auto al= new ArithmeticInstruction(BasicInstruction::FADD, type, (phiins->phi_list)[0].second, new ImmF32Operand(0), phiins->result);
                    newblockmap[endlabel]->InsertInstruction(0,al);
                }
            }
            else if(len >=2)
                newblockmap[endlabel]->InsertInstruction(0,phiins);
        }

        for(auto [id,b] : newblockmap){
            (*C->block_map)[id] = b;
        }
        C->label = nowlabel;
        C->reg = nowreg;
    */
        isupdate=false;
        update=false;
    }
    return isupdate;
}

void InlinePass::FuncInit(){
    for (auto [defI, C] : llvmIR->llvm_cfg) {
        detail d;
        funcdetail[defI->GetFunctionName()]=d;
    }
    for (auto [defI, C] : llvmIR->llvm_cfg) {
        auto& d=funcdetail[defI->GetFunctionName()];
        for(auto [id,b]: *C->block_map){
            for(auto i :b->Instruction_list){
                d.insnum++;
                if(i->GetOpcode()==BasicInstruction::CALL){
                    auto name = ((CallInstruction*)i)->GetFunctionName();
                    if(funcdetail.find(name) != funcdetail.end()){
                        d.iscall=true;
                        funcdetail[name].becalled++;
                    }
                }
            }
        }
    }
}

void InlinePass::Execute(){
    FuncInit();
    bool update = true;
    while(update){
        update = false;
        for(auto [defI,C] : llvmIR->llvm_cfg){
            if(Inline(defI,C))
                update = true;
        }
    }
    llvmIR->BuildCFG();
}