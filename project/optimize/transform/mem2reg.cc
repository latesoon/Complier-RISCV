#include "mem2reg.h"
#include <tuple>

// 检查该条alloca指令是否可以被mem2reg
// eg. 数组不可以mem2reg
// eg. 如果该指针直接被使用不可以mem2reg(在SysY一般不可能发生,SysY不支持指针语法)
bool Mem2RegPass::IsPromotable(CFG *C, Instruction AllocaInst) { 
    //TODO("IsPromotable"); 2024.12.16
    return ((AllocaInstruction *)AllocaInst)->GetDims().empty();
}
/*
    int a1 = 5,a2 = 3,a3 = 11,b = 4
    return b // a1,a2,a3 is useless
-----------------------------------------------
pseudo IR is:
    %r0 = alloca i32 ;a1
    %r1 = alloca i32 ;a2
    %r2 = alloca i32 ;a3
    %r3 = alloca i32 ;b
    store 5 -> %r0 ;a1 = 5
    store 3 -> %r1 ;a2 = 3
    store 11 -> %r2 ;a3 = 11
    store 4 -> %r3 ;b = 4
    %r4 = load i32 %r3
    ret i32 %r4
--------------------------------------------------
%r0,%r1,%r2只有store, 但没有load,所以可以删去
优化后的IR(pseudo)为:
    %r3 = alloca i32
    store 4 -> %r3
    %r4 - load i32 %r3
    ret i32 %r4
*/

// vset is the set of alloca regno that only store but not load
// 该函数对你的时间复杂度有一定要求, 你需要保证你的时间复杂度小于等于O(nlognlogn), n为该函数的指令数
// 提示:deque直接在中间删除是O(n)的, 可以先标记要删除的指令, 最后想一个快速的方法统一删除
void Mem2RegPass::Mem2RegNoUseAlloca(CFG *C, std::set<int> &vset) {
    // this function is used in InsertPhi
    //TODO("Mem2RegNoUseAlloca"); 2024.12.17
    for (auto [id, b] : *C->block_map) {
        std::deque<Instruction> newIns{};
        for (auto i : b->Instruction_list) {
            if ((i->GetOpcode() == BasicInstruction::STORE) && (((StoreInstruction *)i)->GetPointer()->GetOperandType() == BasicOperand::REG)) {
                int now = ((RegOperand *)(((StoreInstruction *)i)->GetPointer()))->GetRegNo();
                if(vset.find(now) != vset.end()) continue;
            }
            else if(i->GetOpcode() == BasicInstruction::ALLOCA && IsPromotable(C,i)){
                int now = ((RegOperand *)(((AllocaInstruction *)i)->GetResult()))->GetRegNo();
                if(vset.find(now) != vset.end()) continue;
            }
            newIns.push_back(i);
        }
        b->Instruction_list=newIns;
    }
}

/*
    int b = getint();
    b = b + 10
    return b // def and use of b are in same block
-----------------------------------------------
pseudo IR is:
    %r0 = alloca i32 ;b
    %r1 = call getint()
    store %r1 -> %r0
    %r2 = load i32 %r0
    %r3 = %r2 + 10
    store %r3 -> %r0
    %r4 = load i32 %r0
    ret i32 %r4
--------------------------------------------------
%r0的所有load和store都在同一个基本块内
优化后的IR(pseudo)为:
    %r1 = call getint()
    %r3 = %r1 + 10
    ret %r3

对于每一个load，我们只需要找到最近的store,然后用store的值替换之后load的结果即可
*/

// vset is the set of alloca regno that load and store are all in the BB block_id
// 该函数对你的时间复杂度有一定要求，你需要保证你的时间复杂度小于等于O(nlognlogn), n为该函数的指令数
void Mem2RegPass::Mem2RegUseDefInSameBlock(CFG *C, std::set<int> &vset, int block_id) {
    // this function is used in InsertPhi
    //TODO("Mem2RegUseDefInSameBlock"); 2024.12.18
    auto b = (*C->block_map)[block_id];
    std::deque<Instruction>newIns{};
    for (auto i : b->Instruction_list) {
        if ((i->GetOpcode() == BasicInstruction::STORE) && (((StoreInstruction *)i)->GetPointer()->GetOperandType() == BasicOperand::REG)) {
            int now = ((RegOperand *)(((StoreInstruction *)i)->GetPointer()))->GetRegNo();
            if(vset.find(now) != vset.end()){
                latest[now] = ((RegOperand *)(((StoreInstruction *)i)->GetValue()));
                continue;
            }
        }
        else if(i->GetOpcode() == BasicInstruction::LOAD && (((LoadInstruction *)i)->GetPointer()->GetOperandType() == BasicOperand::REG)){
            int now = ((RegOperand *)(((LoadInstruction *)i)->GetPointer()))->GetRegNo();
            if(vset.find(now) != vset.end()) {
                auto type = ((LoadInstruction *)i)->GetDataType();
                auto result= ((LoadInstruction *)i)->GetResult();
                if(type == BasicInstruction::I32){
                    auto al= new ArithmeticInstruction(BasicInstruction::ADD, type, latest[now], new ImmI32Operand(0), result);
                    newIns.push_back(al);
                }
                else if(type ==BasicInstruction::FLOAT32){
                    auto al= new ArithmeticInstruction(BasicInstruction::FADD, type, latest[now], new ImmF32Operand(0), result);
                    newIns.push_back(al);
                }
                continue;
            }
        }
        newIns.push_back(i);
    }
    b->Instruction_list=newIns;
}

// vset is the set of alloca regno that one store dominators all load instructions
// 该函数对你的时间复杂度有一定要求，你需要保证你的时间复杂度小于等于O(nlognlogn)
void Mem2RegPass::Mem2RegOneDefDomAllUses(CFG *C, std::set<int> &vset) {
    // this function is used in InsertPhi
    //TODO("Mem2RegOneDefDomAllUses"); 2024.12.18
    for (auto [id, b] : *C->block_map) {
        std::deque<Instruction> newIns{};
        for (auto i : b->Instruction_list) {
            if(i->GetOpcode() == BasicInstruction::LOAD && (((LoadInstruction *)i)->GetPointer()->GetOperandType() == BasicOperand::REG)){
                int now = ((RegOperand *)(((LoadInstruction *)i)->GetPointer()))->GetRegNo();
                if(vset.find(now) != vset.end()) {
                    auto type = ((LoadInstruction *)i)->GetDataType();
                    auto result= ((LoadInstruction *)i)->GetResult();
                    if(type == BasicInstruction::I32){
                        auto al= new ArithmeticInstruction(BasicInstruction::ADD, type, latest[now], new ImmI32Operand(0), result);
                        newIns.push_back(al);
                    }
                    else if(type ==BasicInstruction::FLOAT32){
                        auto al= new ArithmeticInstruction(BasicInstruction::FADD, type, latest[now], new ImmF32Operand(0), result);
                        newIns.push_back(al);
                    }
                    continue;
                }
            }
            else if(i->GetOpcode() == BasicInstruction::ALLOCA && IsPromotable(C,i)){
                int now = ((RegOperand *)(((AllocaInstruction *)i)->GetResult()))->GetRegNo();
                if(vset.find(now) != vset.end()) continue;
            }
            newIns.push_back(i);
        }
        b->Instruction_list=newIns;
    }
}

void Mem2RegPass::InsertPhi(CFG *C) { 
    //TODO("InsertPhi"); 2024.12.19
    auto dc = domtrees->GetDomTree(C);
    phimap.clear();
    phimap.resize(C->label+1);
    latestmap.clear();
    latestmap.resize(C->label+1);
    for (auto [id, b] : *C->block_map) {
        for (auto i : b->Instruction_list) {
            if(i->GetOpcode() == BasicInstruction::ALLOCA && IsPromotable(C,i)){
                
                int r = ((RegOperand *)(((AllocaInstruction *)i)->GetResult()))->GetRegNo();
                
                if(phi.find(r)!=phi.end()){
                    
                    auto list = phi[r].second.store;
                    std::set<int> add{};
                    auto type = phi[r].first;

                    while(!list.empty()){
                        int b1 = *list.begin();
                        list.erase(b1);
                        for(int b2: dc->GetDF(b1)){
                            if(add.find(b2)==add.end()){
                                add.insert(b2);
                                auto op = GetNewRegOperand(++C->reg);
                                PhiInstruction *ins = new PhiInstruction(type, op);
                                phimap[b2][r]=ins;
                                latestmap[b2][r]=op;
                                if(alloc[r].store.find(b2) == alloc[r].store.end()){
                                    list.insert(b2);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Mem2RegPass::VarRename(CFG *C) { 
    //TODO("VarRename"); 2024.12.19
    std::queue<int> worklist{};
    worklist.push(0);
    std::set<int> use{};
    use.insert(0);
    std::set <int> delphi{};
    while(!worklist.empty()){
        //std::cout<<worklist.front();
        int id = worklist.front();
        worklist.pop();
        auto b = (*C->block_map)[id];
        std::deque<Instruction>newIns{};
        for(auto i : b->Instruction_list){
            if(i->GetOpcode() == BasicInstruction::ALLOCA && IsPromotable(C,i)){
                int now = ((RegOperand *)(((AllocaInstruction *)i)->GetResult()))->GetRegNo();
                if(phi.find(now) != phi.end()) continue;
            }
            else if ((i->GetOpcode() == BasicInstruction::STORE) && (((StoreInstruction *)i)->GetPointer()->GetOperandType() == BasicOperand::REG)) {
                int now = ((RegOperand *)(((StoreInstruction *)i)->GetPointer()))->GetRegNo();
                if(phi.find(now) != phi.end()){
                    latestmap[id][now] = ((RegOperand *)(((StoreInstruction *)i)->GetValue()));
                    continue;
                }
            }
            else if(i->GetOpcode() == BasicInstruction::LOAD && (((LoadInstruction *)i)->GetPointer()->GetOperandType() == BasicOperand::REG)){
                int now = ((RegOperand *)(((LoadInstruction *)i)->GetPointer()))->GetRegNo();
                if(phi.find(now) != phi.end()) {
                    auto type = ((LoadInstruction *)i)->GetDataType();
                    auto result= ((LoadInstruction *)i)->GetResult();
                    if(type == BasicInstruction::I32){
                        auto al= new ArithmeticInstruction(BasicInstruction::ADD, type, latestmap[id][now], new ImmI32Operand(0), result);
                        newIns.push_back(al);
                    }
                    else if(type ==BasicInstruction::FLOAT32){
                        auto al= new ArithmeticInstruction(BasicInstruction::FADD, type, latestmap[id][now], new ImmF32Operand(0), result);
                        newIns.push_back(al);
                    }
                    continue;
                }
            }
            newIns.push_back(i);
        }
        b->Instruction_list=newIns;
        for(auto b1 : C->G[id]){
            //std::cout<<b1->block_id;
            int idb1=b1->block_id;
            if(use.count(idb1)) {
            //    for(auto [num,op]: latestmap[id]){
            //        if(phimap[idb1].find(num) != phimap[idb1].end())
            //            (phimap[idb1][num]->phi_list).emplace_back(GetNewLabelOperand(id),latestmap[id][num]);
            //    }
                std::vector<int> to_del{};
                for(auto [num,op]:phimap[idb1]){
                    if(latestmap[id].find(num)!=latestmap[id].end()){
                        (phimap[idb1][num]->phi_list).emplace_back(GetNewLabelOperand(id),latestmap[id][num]);
                    }
                    else{
                        to_del.push_back(num);
                        delphi.insert(((RegOperand *)(phimap[idb1][num]->result))->GetRegNo());
                    }
                }
                for(int delnum:to_del)
                    phimap[idb1].erase(delnum);

                continue;//already in
            }
            for(auto [num,op]: latestmap[id]){
                if(latestmap[idb1].find(num) == latestmap[idb1].end())
                    latestmap[idb1][num]=latestmap[id][num];
                //else{//if find ,must in phimap
                    //(phimap[idb1][num]->phi_list).emplace_back(GetNewLabelOperand(id),latestmap[id][num]);
                //}
            }
            std::vector<int> to_del{};
            for(auto [num,op]:phimap[idb1]){
                if(latestmap[id].find(num)!=latestmap[id].end()){
                    (phimap[idb1][num]->phi_list).emplace_back(GetNewLabelOperand(id),latestmap[id][num]);
                }
                else{
                    to_del.push_back(num);
                    delphi.insert(((RegOperand *)(phimap[idb1][num]->result))->GetRegNo());
                }
            }
            for(int delnum:to_del)
                phimap[idb1].erase(delnum);
            worklist.push(idb1);
            use.insert(idb1);
        }
    }
    bool update =true;
    while(update){
        update = false;
        for(int id : use){
            std::vector<int> to_del{};
            for(auto [num,ins]:phimap[id]){
                bool flag=true;
                for(auto i:ins->phi_list){
                    if(delphi.find(((RegOperand *)(i.second))->GetRegNo())!=delphi.end()){
                        flag = false;
                        update = true;
                        break;
                    }
                }
                if(!flag){
                    to_del.push_back(num);
                    delphi.insert(((RegOperand *)(phimap[id][num]->result))->GetRegNo());
                }
            }
            for(int delnum:to_del)
                phimap[id].erase(delnum);
        }
    }
    for(int id : use){//insert phi
        auto b = (*C->block_map)[id];
        for(auto [num,ins]:phimap[id]){
            int len=ins->phi_list.size();
            if(len == 1){
                auto type=ins->type;
                if(type == BasicInstruction::I32){
                    auto al= new ArithmeticInstruction(BasicInstruction::ADD, type, (ins->phi_list)[0].second, new ImmI32Operand(0), ins->result);
                    b->InsertInstruction(0,al);
                }
                else if(type ==BasicInstruction::FLOAT32){
                    auto al= new ArithmeticInstruction(BasicInstruction::FADD, type, (ins->phi_list)[0].second, new ImmF32Operand(0), ins->result);
                    b->InsertInstruction(0,al);
                }
            }
            else if(len >=2)
                b->InsertInstruction(0,ins);
        }
    }
}

//add 2024.12.17
void Mem2RegPass::Allocconstruct(CFG *C){
    alloc.clear();
    phi.clear();
    auto dc = domtrees->GetDomTree(C);
    //if(dc != nullptr)std::cout<<1;
    for (auto [id, b] : *C->block_map) {
        for (auto i : b->Instruction_list) {
            if ((i->GetOpcode() == BasicInstruction::STORE) && (((StoreInstruction *)i)->GetPointer()->GetOperandType() == BasicOperand::REG)) {
                auto& now = alloc[((RegOperand *)(((StoreInstruction *)i)->GetPointer()))->GetRegNo()];
                now.store.insert(id);
            }
            else if ((i->GetOpcode() == BasicInstruction::LOAD) && (((LoadInstruction *)i)->GetPointer()->GetOperandType() == BasicOperand::REG)){
                auto& now = alloc[((RegOperand *)(((LoadInstruction *)i)->GetPointer()))->GetRegNo()];
                now.load.insert(id);
            }
        }
    }

    std::set<int> nouse;
    std::map<int,std::set<int> >sameblock;
    std::set<int> onedef;

    for (auto [id, b] : *C->block_map) {
        for (auto i : b->Instruction_list) {
            if(i->GetOpcode() == BasicInstruction::ALLOCA && IsPromotable(C,i)){
                
                int r = ((RegOperand *)(((AllocaInstruction *)i)->GetResult()))->GetRegNo();
                
                //std::cout<<r<<' '<<alloc[r].load.size()<<' '<<alloc[r].store.size();
                if (alloc[r].load.empty()){
                    nouse.insert(r);
                }
                else if (alloc[r].load.size()==1 && alloc[r].store.size()==1 && *(alloc[r].load.begin()) == *(alloc[r].store.begin())){
                    sameblock[*(alloc[r].load.begin())].insert(r);
                    onedef.insert(r);
                    //std::cout<<*(alloc[r].load.begin())<<' '<<r;
                }
                else{ 
                    if (alloc[r].store.size()== 1){
                        int s=*(alloc[r].store.begin());
                        bool flag = true;
                        for (int l : alloc[r].load) {
                            //std::cout<<s<<l;
                            //std::cout<<dc->IsDominate(1,2);
                            if (!(dc->IsDominate(s,l))) {
                                flag = false;
                                break;
                            }
                        }
                        if(flag){
                            sameblock[s].insert(r);
                            onedef.insert(r);
                            continue;
                        }
                    }

                    //common insert phi
                    auto type = ((AllocaInstruction *)i)->GetDataType();
                    phi[r]=std::make_pair(type,alloc[r]);
                }
            }
        }
    }

    Mem2RegNoUseAlloca(C,nouse);
    for (auto [id, vset] : sameblock)
        Mem2RegUseDefInSameBlock(C, vset, id);
    Mem2RegOneDefDomAllUses(C,onedef);

    latest.clear();
}

void Mem2RegPass::Mem2Reg(CFG *C) {
    //std::cout<<111;
    Allocconstruct(C);
    InsertPhi(C);
    VarRename(C);
}

void Mem2RegPass::Execute() {
    for (auto [defI, cfg] : llvmIR->llvm_cfg) {
        Mem2Reg(cfg);
    }
}
