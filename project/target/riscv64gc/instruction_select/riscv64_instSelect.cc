#include "riscv64_instSelect.h"
#include <sstream>

Register RiscV64Selector::GetReg(int regno, MachineDataType ty){
    if(register_table.find(regno)==register_table.end())
        register_table[regno] = cur_func->GetNewRegister(ty.data_type, ty.data_length);
    return register_table[regno];
}

template <> void RiscV64Selector::ConvertAndAppend<LoadInstruction *>(LoadInstruction *ins) {
    //TODO("Implement this if you need");
    auto ptr = ins->GetPointer();
    int resultno = ((RegOperand*)ins->GetResult())->GetRegNo();
    if (ptr->GetOperandType() == BasicOperand::REG) {//in function
        int ptrno = ((RegOperand*)ptr)->GetRegNo();
        if(alloca_table.find(ptrno)==alloca_table.end()){//not alloca
            Register r_ptr = GetReg(ptrno, INT64);
            if (ins->GetDataType() == BasicInstruction::I32) {
                Register r_result = GetReg(resultno, INT64);
                cur_block->push_back(rvconstructor->ConstructIImm(RISCV_LW, r_result, r_ptr, 0));
            }
            else if(ins->GetDataType() == BasicInstruction::FLOAT32){
                Register r_result = GetReg(resultno, FLOAT64);
                cur_block->push_back(rvconstructor->ConstructIImm(RISCV_FLW, r_result, r_ptr, 0));
            }
            else if(ins->GetDataType() == BasicInstruction::PTR){
                Register r_result = GetReg(resultno, INT64);
                cur_block->push_back(rvconstructor->ConstructIImm(RISCV_LD, r_result, r_ptr, 0));
            }
        }
        else{//alloca
            if (ins->GetDataType() == BasicInstruction::I32) {
                Register r_result = GetReg(resultno, INT64);
                cur_block->push_back(rvconstructor->ConstructIImm(RISCV_LW, r_result, GetPhysicalReg(RISCV_sp), alloca_table[ptrno]));
            }
            else if(ins->GetDataType() == BasicInstruction::FLOAT32){
                Register r_result = GetReg(resultno, FLOAT64);
                cur_block->push_back(rvconstructor->ConstructIImm(RISCV_FLW, r_result, GetPhysicalReg(RISCV_sp), alloca_table[ptrno]));
            }
            else if(ins->GetDataType() == BasicInstruction::PTR){
                Register r_result = GetReg(resultno, INT64);
                cur_block->push_back(rvconstructor->ConstructIImm(RISCV_LD, r_result, GetPhysicalReg(RISCV_sp), alloca_table[ptrno]));
            }
        }
    }
    else if(ptr->GetOperandType() == BasicOperand::GLOBAL){//global
        auto name = ((GlobalOperand *)(ins->GetPointer()))->GetName();
        Register tmp = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
        if (ins->GetDataType() == BasicInstruction::I32) {
            Register r_result = GetReg(resultno, INT64);
            cur_block->push_back(rvconstructor->ConstructULabel(RISCV_LUI, tmp, RiscVLabel(name, true)));
            cur_block->push_back(rvconstructor->ConstructILabel(RISCV_LW, r_result, tmp, RiscVLabel(name, false)));
        }
        else if (ins->GetDataType() == BasicInstruction::FLOAT32) {
            Register r_result = GetReg(resultno, FLOAT64);
            cur_block->push_back(rvconstructor->ConstructULabel(RISCV_LUI, tmp, RiscVLabel(name, true)));
            cur_block->push_back(rvconstructor->ConstructILabel(RISCV_FLW, r_result, tmp, RiscVLabel(name, false)));
        }
        else if (ins->GetDataType() == BasicInstruction::PTR) {
            Register r_result = GetReg(resultno, INT64);
            cur_block->push_back(rvconstructor->ConstructULabel(RISCV_LUI, tmp, RiscVLabel(name, true)));
            cur_block->push_back(rvconstructor->ConstructILabel(RISCV_LD, r_result, tmp, RiscVLabel(name, false)));
        } 
    }
}

template <> void RiscV64Selector::ConvertAndAppend<StoreInstruction *>(StoreInstruction *ins) {
    //TODO("Implement this if you need");
    Register r_value;
    auto value = ins->GetValue();
    if (value->GetOperandType() == BasicOperand::IMMI32) {
        int val= ((ImmI32Operand *)value)->GetIntImmVal();
        r_value = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
        cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, r_value, GetPhysicalReg(RISCV_x0),val));
    }
    else if (value->GetOperandType() == BasicOperand::IMMF32){
        float val= ((ImmF32Operand *)value)->GetFloatVal();
        r_value = cur_func->GetNewRegister(FLOAT64.data_type, FLOAT64.data_length);
        uint32_t binary = *reinterpret_cast<uint32_t*>(&val);
        Register temp_int = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
        cur_block->push_back(rvconstructor->ConstructUImm(RISCV_LUI, temp_int, (binary >> 12)));  // 高 20 位
        cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, temp_int, temp_int, (binary & 0xFFF)));  // 低 12 位
        cur_block->push_back(rvconstructor->ConstructR2(RISCV_FMV_W_X, r_value, temp_int));
    }
    else if (value->GetOperandType() == BasicOperand::REG) {
        auto valno = ((RegOperand *)value)->GetRegNo();
        if (ins->GetDataType() == BasicInstruction::I32)
            r_value = GetReg(valno, INT64);
        else if (ins->GetDataType() == BasicInstruction::FLOAT32)
            r_value = GetReg(valno, FLOAT64);
    }
    auto ptr = ins->GetPointer();
    if (ptr->GetOperandType() == BasicOperand::REG) {
        int ptrno = ((RegOperand*)ptr)->GetRegNo();
        if(alloca_table.find(ptrno)==alloca_table.end()){//not alloca
            auto r_ptr = GetReg(ptrno, INT64);
            if(ins->GetDataType() == BasicInstruction::I32)
                cur_block->push_back(rvconstructor->ConstructSImm(RISCV_SW, r_value, r_ptr, 0));
            else if(ins->GetDataType() == BasicInstruction::FLOAT32)
                cur_block->push_back(rvconstructor->ConstructSImm(RISCV_FSW, r_value, r_ptr, 0));
        }
        else{
            if(ins->GetDataType() == BasicInstruction::I32)
                cur_block->push_back(rvconstructor->ConstructSImm(RISCV_SW, r_value, GetPhysicalReg(RISCV_sp), alloca_table[ptrno]));
            else if(ins->GetDataType() == BasicInstruction::FLOAT32)
                cur_block->push_back(rvconstructor->ConstructSImm(RISCV_FSW, r_value, GetPhysicalReg(RISCV_sp), alloca_table[ptrno]));
        }
    }
    else if(ptr->GetOperandType() == BasicOperand::GLOBAL){
        auto name = ((GlobalOperand *)(ins->GetPointer()))->GetName();
        Register tmp = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
        if (ins->GetDataType() == BasicInstruction::I32) {
            cur_block->push_back(rvconstructor->ConstructULabel(RISCV_LUI, tmp, RiscVLabel(name, true)));
            cur_block->push_back(rvconstructor->ConstructSLabel(RISCV_SW, r_value, tmp, RiscVLabel(name, false)));
        }
        else if (ins->GetDataType() == BasicInstruction::FLOAT32) {
            cur_block->push_back(rvconstructor->ConstructULabel(RISCV_LUI, tmp, RiscVLabel(name, true)));
            cur_block->push_back(rvconstructor->ConstructSLabel(RISCV_FSW, r_value, tmp, RiscVLabel(name, false)));
        }
    }
}

template <> void RiscV64Selector::ConvertAndAppend<ArithmeticInstruction *>(ArithmeticInstruction *ins) {
    //TODO("Implement this if you need");
    auto op = ins->GetOpcode();
    if (op == BasicInstruction::ADD || op == BasicInstruction::SUB || op == BasicInstruction::MUL || op == BasicInstruction::DIV ||op == BasicInstruction::MOD){
        Register rd = GetReg(((RegOperand *)ins->GetResult())->GetRegNo(), INT64);
        Register rs,rt;
        auto rstype = ins->GetOperand1()->GetOperandType();
        if(rstype == BasicOperand::IMMI32){
            int val = ((ImmI32Operand *)ins->GetOperand1())->GetIntImmVal();
            rs = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
            cur_block->push_back(rvconstructor->ConstructUImm(RISCV_LI, rs, val));
            //cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, rs, GetPhysicalReg(RISCV_x0),val));
        }
        else if(rstype == BasicOperand::REG)
            rs = GetReg(((RegOperand *)ins->GetOperand1())->GetRegNo(), INT64);
        auto rttype = ins->GetOperand2()->GetOperandType();
        if(rttype == BasicOperand::IMMI32){
            int val = ((ImmI32Operand *)ins->GetOperand2())->GetIntImmVal();
            rt = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
            cur_block->push_back(rvconstructor->ConstructUImm(RISCV_LI, rt, val));
            //cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, rt, GetPhysicalReg(RISCV_x0),val));
        }
        else if(rttype == BasicOperand::REG){
            rt = GetReg(((RegOperand *)ins->GetOperand2())->GetRegNo(), INT64);
        }
        if(op == BasicInstruction::ADD)
            cur_block->push_back(rvconstructor->ConstructR(RISCV_ADDW, rd, rs, rt));
        else if(op == BasicInstruction::SUB)
            cur_block->push_back(rvconstructor->ConstructR(RISCV_SUBW, rd, rs, rt));
        else if(op == BasicInstruction::MUL)
            cur_block->push_back(rvconstructor->ConstructR(RISCV_MULW, rd, rs, rt));
        else if(op == BasicInstruction::DIV)
            cur_block->push_back(rvconstructor->ConstructR(RISCV_DIVW, rd, rs, rt));
        else
            cur_block->push_back(rvconstructor->ConstructR(RISCV_REMW, rd, rs, rt));
    }
    else if(op == BasicInstruction::FADD || op == BasicInstruction::FSUB || op == BasicInstruction::FMUL || op == BasicInstruction::FDIV){
        Register rd = GetReg(((RegOperand *)ins->GetResult())->GetRegNo(), FLOAT64);
        Register rs,rt;
        auto rstype = ins->GetOperand1()->GetOperandType();
        if(rstype == BasicOperand::IMMF32){
            float val= ((ImmF32Operand *)ins->GetOperand1())->GetFloatVal();
            rs = cur_func->GetNewRegister(FLOAT64.data_type, FLOAT64.data_length);
            uint32_t binary = *reinterpret_cast<uint32_t*>(&val);
            Register temp_int = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
            cur_block->push_back(rvconstructor->ConstructUImm(RISCV_LUI, temp_int, (binary >> 12)));  // 高 20 位
            cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, temp_int, temp_int, (binary & 0xFFF)));  // 低 12 位
            cur_block->push_back(rvconstructor->ConstructR2(RISCV_FMV_W_X, rs, temp_int));
        }
        else if(rstype == BasicOperand::REG)
            rs = GetReg(((RegOperand *)ins->GetOperand1())->GetRegNo(), FLOAT64);
        auto rttype = ins->GetOperand2()->GetOperandType();
        if(rttype == BasicOperand::IMMF32){
            float val= ((ImmF32Operand *)ins->GetOperand2())->GetFloatVal();
            rt = cur_func->GetNewRegister(FLOAT64.data_type, FLOAT64.data_length);
            uint32_t binary = *reinterpret_cast<uint32_t*>(&val);
            Register temp_int = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
            cur_block->push_back(rvconstructor->ConstructUImm(RISCV_LUI, temp_int, (binary >> 12)));  // 高 20 位
            cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, temp_int, temp_int, (binary & 0xFFF)));  // 低 12 位
            cur_block->push_back(rvconstructor->ConstructR2(RISCV_FMV_W_X, rt, temp_int));
        }
        else if(rttype == BasicOperand::REG)
            rt = GetReg(((RegOperand *)ins->GetOperand2())->GetRegNo(), FLOAT64);
        if(op == BasicInstruction::ADD)
            cur_block->push_back(rvconstructor->ConstructR(RISCV_FADD_S, rd, rs, rt));
        else if(op == BasicInstruction::SUB)
            cur_block->push_back(rvconstructor->ConstructR(RISCV_FSUB_S, rd, rs, rt));
        else if(op == BasicInstruction::MUL)
            cur_block->push_back(rvconstructor->ConstructR(RISCV_FMUL_S, rd, rs, rt));
        else if(op == BasicInstruction::DIV)
            cur_block->push_back(rvconstructor->ConstructR(RISCV_FDIV_S, rd, rs, rt));
    }
}

template <> void RiscV64Selector::ConvertAndAppend<IcmpInstruction *>(IcmpInstruction *ins) {
    //TODO("Implement this if you need");
    auto op1 = ins->GetOp1();
    auto op2 = ins->GetOp2();
    int resno = ((RegOperand *)ins->GetResult())->GetRegNo();
    Register r_op1,r_op2,r_res;
    r_res = GetReg(resno,INT64);
    if (op1->GetOperandType() == BasicOperand::REG)
        r_op1 = GetReg(((RegOperand *)op1)->GetRegNo(), INT64);
    else if(op1->GetOperandType() == BasicOperand::IMMI32){
        int val = ((ImmI32Operand *)op1)->GetIntImmVal();
        r_op1 = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
        cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, r_op1, GetPhysicalReg(RISCV_x0),val));
    }
    if (op2->GetOperandType() == BasicOperand::REG)
        r_op2 = GetReg(((RegOperand *)op2)->GetRegNo(), INT64);
    else if(op2->GetOperandType() == BasicOperand::IMMI32){
        int val = ((ImmI32Operand *)op2)->GetIntImmVal();
        r_op2 = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
        cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, r_op2, GetPhysicalReg(RISCV_x0),val));
    }
    switch (ins->GetCond()) {
        case BasicInstruction::eq:
            cur_block->push_back(rvconstructor->ConstructR(RISCV_SUBW, r_res, r_op1, r_op2));
            cur_block->push_back(rvconstructor->ConstructIImm(RISCV_SLTIU, r_res, r_res, 1));
            break;
        case BasicInstruction::ne:
            cur_block->push_back(rvconstructor->ConstructR(RISCV_SUBW, r_res, r_op1, r_op2));
            cur_block->push_back(rvconstructor->ConstructR(RISCV_SLTU, r_res, GetPhysicalReg(RISCV_x0), r_res));
            break;
        case BasicInstruction::slt:
            cur_block->push_back(rvconstructor->ConstructR(RISCV_SLT, r_res, r_op1, r_op2));
            break;
        case BasicInstruction::sgt:
            cur_block->push_back(rvconstructor->ConstructR(RISCV_SLT, r_res, r_op2, r_op1));
            break;
        case BasicInstruction::sle:
            cur_block->push_back(rvconstructor->ConstructR(RISCV_SLT, r_res, r_op2, r_op1));
            cur_block->push_back(rvconstructor->ConstructIImm(RISCV_SLTIU, r_res, r_res, 1));
            break;
        case BasicInstruction::sge:
            cur_block->push_back(rvconstructor->ConstructR(RISCV_SLT, r_res, r_op1, r_op2));
            cur_block->push_back(rvconstructor->ConstructIImm(RISCV_SLTIU, r_res, r_res, 1));
            break;
        default:
            break;
    }
}

template <> void RiscV64Selector::ConvertAndAppend<FcmpInstruction *>(FcmpInstruction *ins) {
    //TODO("Implement this if you need");
    auto op1 = ins->GetOp1();
    auto op2 = ins->GetOp2();
    int resno = ((RegOperand*)ins->GetResult())->GetRegNo();
    Register r_op1,r_op2,r_res;
    r_res = GetReg(resno,INT64);
    if (op1->GetOperandType() == BasicOperand::REG)
        r_op1 = GetReg(((RegOperand *)op1)->GetRegNo(), FLOAT64);
    else if (op1->GetOperandType() == BasicOperand::IMMF32) {
        r_op1 = cur_func->GetNewRegister(FLOAT64.data_type, FLOAT64.data_length);
        float val = ((ImmF32Operand *)op1)->GetFloatVal();
        uint32_t binary = *reinterpret_cast<uint32_t*>(&val);
        Register temp_int = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
        cur_block->push_back(rvconstructor->ConstructUImm(RISCV_LUI, temp_int, (binary >> 12)));  // 高 20 位
        cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, temp_int, temp_int, (binary & 0xFFF)));  // 低 12 位
        cur_block->push_back(rvconstructor->ConstructR2(RISCV_FMV_W_X, r_op1, temp_int));
    }
    if (op2->GetOperandType() == BasicOperand::REG) {
        r_op2 = GetReg(((RegOperand *)op2)->GetRegNo(), FLOAT64);
    }
    else if (op2->GetOperandType() == BasicOperand::IMMF32) {
        r_op2 = cur_func->GetNewRegister(FLOAT64.data_type, FLOAT64.data_length);
        float val = ((ImmF32Operand *)op2)->GetFloatVal();
        uint32_t binary = *reinterpret_cast<uint32_t*>(&val);
        Register temp_int = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
        cur_block->push_back(rvconstructor->ConstructUImm(RISCV_LUI, temp_int, (binary >> 12)));  // 高 20 位
        cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, temp_int, temp_int, (binary & 0xFFF)));  // 低 12 位
        cur_block->push_back(rvconstructor->ConstructR2(RISCV_FMV_W_X, r_op2, temp_int));
    }
    switch (ins->GetCond()) {
        case BasicInstruction::OEQ:
        case BasicInstruction::UEQ:
            cur_block->push_back(rvconstructor->ConstructR(RISCV_FEQ_S, r_res, r_op1, r_op2));
            break;
        case BasicInstruction::ONE:
        case BasicInstruction::UNE:
            cur_block->push_back(rvconstructor->ConstructR(RISCV_FEQ_S, r_res, r_op1, r_op2));
            cur_block->push_back(rvconstructor->ConstructIImm(RISCV_SLTIU, r_res, r_res, 1));
            break;
        case BasicInstruction::OLT:
        case BasicInstruction::ULT:
            cur_block->push_back(rvconstructor->ConstructR(RISCV_FLT_S, r_res, r_op1, r_op2));
            break;
        case BasicInstruction::OGT:
        case BasicInstruction::UGT:
            cur_block->push_back(rvconstructor->ConstructR(RISCV_FLT_S, r_res, r_op2, r_op1));
            break;
        case BasicInstruction::OLE:
        case BasicInstruction::ULE:
            cur_block->push_back(rvconstructor->ConstructR(RISCV_FLE_S, r_res, r_op1, r_op2));
            break;
        case BasicInstruction::OGE:
        case BasicInstruction::UGE:
            cur_block->push_back(rvconstructor->ConstructR(RISCV_FLE_S, r_res, r_op2, r_op1));
            break;
        default:
            break;
    }
}

template <> void RiscV64Selector::ConvertAndAppend<AllocaInstruction *>(AllocaInstruction *ins) {
    //TODO("Implement this if you need");
    int opno = ((RegOperand *)ins->GetResult())->GetRegNo();
    int size = ins->GetSize() << 2;
    alloca_table[opno] = cur_offset;
    cur_offset += size;
}

template <> void RiscV64Selector::ConvertAndAppend<BrCondInstruction *>(BrCondInstruction *ins) {
    //TODO("Implement this if you need");
    int cmpno =((RegOperand *)ins->GetCond())->GetRegNo();
    Register r_cmp = GetReg(cmpno,INT64);
    auto tlabel = RiscVLabel(((LabelOperand *)ins->GetTrueLabel())->GetLabelNo());
    auto flabel = RiscVLabel(((LabelOperand *)ins->GetFalseLabel())->GetLabelNo());
    cur_block->push_back(rvconstructor->ConstructBLabel(RISCV_BNE,r_cmp,GetPhysicalReg(RISCV_x0),tlabel));
    cur_block->push_back(rvconstructor->ConstructJLabel(RISCV_JAL, GetPhysicalReg(RISCV_x0), flabel));
}

template <> void RiscV64Selector::ConvertAndAppend<BrUncondInstruction *>(BrUncondInstruction *ins) {
    //TODO("Implement this if you need");
    auto dst = RiscVLabel(((LabelOperand *)ins->GetDestLabel())->GetLabelNo());
    cur_block->push_back(rvconstructor->ConstructJLabel(RISCV_JAL, GetPhysicalReg(RISCV_x0), dst));
}

template <> void RiscV64Selector::ConvertAndAppend<CallInstruction *>(CallInstruction *ins) {
    //TODO("Implement this if you need");
    std::string funcname = ins->GetFunctionName();
    bool ismemset = false;
    if(funcname == std::string("llvm.memset.p0.i32")){
        funcname = std::string("memset");
        ismemset = true;
    }
    int ireg = 0, freg = 0, argoffset = 0;
    for (auto [type, arg] : ins->GetParameterList()) {
        if (type != BasicInstruction::FLOAT32){
            if(arg->GetOperandType() == BasicOperand::IMMI32) {
                int val = ((ImmI32Operand *)arg)->GetIntImmVal();
                //Register r_arg = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
                //cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, r_arg, GetPhysicalReg(RISCV_x0),val));
                //cur_block->push_back(rvconstructor->ConstructSImm(RISCV_SD, r_arg, GetPhysicalReg(RISCV_sp), -argoffset));
                if(ismemset || type == BasicInstruction::PTR){
                    cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDI, GetPhysicalReg(RISCV_a7), GetPhysicalReg(RISCV_x0),val));
                    cur_block->push_back(rvconstructor->ConstructSImm(RISCV_SD, GetPhysicalReg(RISCV_a7), GetPhysicalReg(RISCV_sp), argoffset));
                }
                else{
                    cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, GetPhysicalReg(RISCV_a7), GetPhysicalReg(RISCV_x0),val));
                    cur_block->push_back(rvconstructor->ConstructSImm(RISCV_SW, GetPhysicalReg(RISCV_a7), GetPhysicalReg(RISCV_sp), argoffset));
                }
                
            }
            else if(arg->GetOperandType() == BasicOperand::REG){
                int valno = ((RegOperand *)arg)->GetRegNo();
                Register r_val = GetReg(valno, INT64);
                if(alloca_table.find(valno) == alloca_table.end()){
                    //if(ismemset || type == BasicInstruction::PTR)
                    //    cur_block->push_back(rvconstructor->ConstructSImm(RISCV_SD, r_val, GetPhysicalReg(RISCV_sp), argoffset));
                    //else
                        cur_block->push_back(rvconstructor->ConstructSImm(RISCV_SD, r_val, GetPhysicalReg(RISCV_sp), argoffset));
                }
                else{
                    //if(ismemset || type == BasicInstruction::PTR){
                        cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDI, GetPhysicalReg(RISCV_a7), GetPhysicalReg(RISCV_sp), alloca_table[valno]));
                        cur_block->push_back(rvconstructor->ConstructSImm(RISCV_SD, GetPhysicalReg(RISCV_a7), GetPhysicalReg(RISCV_sp), argoffset));
                    //}
                    //else{
                    //    cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDI, GetPhysicalReg(RISCV_a7), GetPhysicalReg(RISCV_sp), alloca_table[valno]));
                    //    cur_block->push_back(rvconstructor->ConstructSImm(RISCV_SD, GetPhysicalReg(RISCV_a7), GetPhysicalReg(RISCV_sp), argoffset));
                    //}
                }
            }
            else if(arg->GetOperandType() == BasicOperand::GLOBAL){
                auto name = ((GlobalOperand *)arg)->GetName();
                cur_block->push_back(rvconstructor->ConstructULabel(RISCV_LUI, GetPhysicalReg(RISCV_a7), RiscVLabel(name, true)));
                cur_block->push_back(rvconstructor->ConstructILabel(RISCV_ADDI, GetPhysicalReg(RISCV_a7), GetPhysicalReg(RISCV_a7), RiscVLabel(name, false)));
                cur_block->push_back(rvconstructor->ConstructSImm(RISCV_SD, GetPhysicalReg(RISCV_a7), GetPhysicalReg(RISCV_sp), argoffset));
            }
            if(ireg < 3){
                //if(ismemset || type == BasicInstruction::PTR)
                //    cur_block->push_back(rvconstructor->ConstructIImm(RISCV_LD, GetPhysicalReg(RISCV_a0 + ireg), GetPhysicalReg(RISCV_sp),argoffset));
                //else
                    cur_block->push_back(rvconstructor->ConstructIImm(RISCV_LD, GetPhysicalReg(RISCV_a0 + ireg), GetPhysicalReg(RISCV_sp),argoffset));
            }
            argoffset += 8;
            ireg++;
        }
        else{
            if(arg->GetOperandType() == BasicOperand::IMMF32) {
                float val= ((ImmF32Operand *)arg)->GetFloatVal();
                uint32_t binary = *reinterpret_cast<uint32_t*>(&val);
                Register temp_int = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
                cur_block->push_back(rvconstructor->ConstructUImm(RISCV_LUI, GetPhysicalReg(RISCV_a7), (binary >> 12)));  // 高 20 位
                cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDI, GetPhysicalReg(RISCV_a7), GetPhysicalReg(RISCV_a7), (binary & 0xFFF)));  // 低 12 位
                cur_block->push_back(rvconstructor->ConstructR2(RISCV_FMV_W_X,  GetPhysicalReg(RISCV_fa7), GetPhysicalReg(RISCV_a7)));
                cur_block->push_back(rvconstructor->ConstructSImm(RISCV_FSW, GetPhysicalReg(RISCV_fa7), GetPhysicalReg(RISCV_sp), argoffset));
            }
            else if(arg->GetOperandType() == BasicOperand::REG){
                int valno = ((RegOperand *)arg)->GetRegNo();
                Register r_val = GetReg(valno, INT64);
                cur_block->push_back(rvconstructor->ConstructSImm(RISCV_FSW, r_val, GetPhysicalReg(RISCV_sp), argoffset));
            }
            if(freg < 3)
                cur_block->push_back(rvconstructor->ConstructIImm(RISCV_FLW, GetPhysicalReg(RISCV_fa0 + freg), GetPhysicalReg(RISCV_sp),argoffset));
            argoffset += 8;
            freg++;
        }
    }
    cur_func->UpdateParaSize(argoffset);
    if(ireg >= 3) ireg = 3;
    if(freg >= 3) freg = 3;
    cur_block->push_back(rvconstructor->ConstructCall(RISCV_CALL, funcname, ireg, freg));
    auto rettype = ins->GetRetType();
    int retno = ins->getretno();
    if (rettype == BasicInstruction::I32)
        cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, GetReg(retno, INT64), GetPhysicalReg(RISCV_a0),0));
    else if (rettype == BasicInstruction::FLOAT32){
        cur_block->push_back(rvconstructor->ConstructR2(RISCV_FMV_W_X,  GetPhysicalReg(RISCV_fa1), GetPhysicalReg(RISCV_x0)));
        cur_block->push_back(rvconstructor->ConstructR(RISCV_FADD_S, GetReg(retno, FLOAT64), GetPhysicalReg(RISCV_fa0), GetPhysicalReg(RISCV_fa1)));
    }
}

template <> void RiscV64Selector::ConvertAndAppend<RetInstruction *>(RetInstruction *ins) {
    if (ins->GetRetVal() != NULL) {
        if (ins->GetRetVal()->GetOperandType() == BasicOperand::IMMI32) {
            auto retimm_op = (ImmI32Operand *)ins->GetRetVal();
            auto imm = retimm_op->GetIntImmVal();

            auto retcopy_instr = rvconstructor->ConstructUImm(RISCV_LI, GetPhysicalReg(RISCV_a0), imm);
            cur_block->push_back(retcopy_instr);
        } else if (ins->GetRetVal()->GetOperandType() == BasicOperand::IMMF32) {
            //TODO("Implement this if you need");
            auto retimm_op = (ImmF32Operand *)ins->GetRetVal();
            auto imm = retimm_op->GetFloatVal();
            uint32_t binary = *reinterpret_cast<uint32_t*>(&imm);
            Register temp_int = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
            cur_block->push_back(rvconstructor->ConstructUImm(RISCV_LUI, temp_int, (binary >> 12)));  // 高 20 位
            cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, temp_int, temp_int, (binary & 0xFFF)));  // 低 12 位
            cur_block->push_back(rvconstructor->ConstructR2(RISCV_FMV_W_X, GetPhysicalReg(RISCV_fa0), temp_int));
        } else if (ins->GetRetVal()->GetOperandType() == BasicOperand::REG) {
            //TODO("Implement this if you need");
            auto regno = ((RegOperand *)ins->GetRetVal())->GetRegNo();
            if(ins->GetType() == BasicInstruction::I32) {
                auto r_reg = GetReg(regno, INT64);
                cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, GetPhysicalReg(RISCV_a0), r_reg, 0));
            }
            else if(ins->GetType() == BasicInstruction::FLOAT32) {
                auto r_reg = GetReg(regno, FLOAT64);
                Register temp_int = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
                Register r_f = cur_func->GetNewRegister(FLOAT64.data_type, FLOAT64.data_length);
                cur_block->push_back(rvconstructor->ConstructUImm(RISCV_LI, temp_int, 0));
                cur_block->push_back(rvconstructor->ConstructR2(RISCV_FMV_W_X, r_f, temp_int));
                cur_block->push_back(rvconstructor->ConstructR(RISCV_FADD_S, GetPhysicalReg(RISCV_fa0), r_f, r_reg));
            }
        }
    }

    cur_block->push_back(rvconstructor->ConstructR(RISCV_ADD, GetPhysicalReg(RISCV_sp), GetPhysicalReg(RISCV_s0), GetPhysicalReg(RISCV_x0)));
    cur_block->push_back(rvconstructor->ConstructIImm(RISCV_LD, GetPhysicalReg(RISCV_ra), GetPhysicalReg(RISCV_s0), -8));
    cur_block->push_back(rvconstructor->ConstructIImm(RISCV_LD, GetPhysicalReg(RISCV_s0), GetPhysicalReg(RISCV_s0), -16));
    
    

    auto ret_instr = rvconstructor->ConstructIImm(RISCV_JALR, GetPhysicalReg(RISCV_x0), GetPhysicalReg(RISCV_ra), 0);
    if (ins->GetType() == BasicInstruction::I32) {
        ret_instr->setRetType(1);
    } else if (ins->GetType() == BasicInstruction::FLOAT32) {
        ret_instr->setRetType(2);
    } else {
        ret_instr->setRetType(0);
    }
    cur_block->push_back(ret_instr);
}

template <> void RiscV64Selector::ConvertAndAppend<FptosiInstruction *>(FptosiInstruction *ins) {
    //TODO("Implement this if you need");
    auto src = ins->GetSrc();
    int dstno = ((RegOperand*)ins->GetResult())->GetRegNo();
    Register r_src,r_dst;
    r_dst = GetReg(dstno, INT64);
    if (src->GetOperandType() == BasicOperand::IMMF32) {
        float val= ((ImmF32Operand *)src)->GetFloatVal();
        r_src = cur_func->GetNewRegister(FLOAT64.data_type, FLOAT64.data_length);
        uint32_t binary = *reinterpret_cast<uint32_t*>(&val);
        Register temp_int = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
        cur_block->push_back(rvconstructor->ConstructUImm(RISCV_LUI, temp_int, (binary >> 12)));  // 高 20 位
        cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, temp_int, temp_int, (binary & 0xFFF)));  // 低 12 位
        cur_block->push_back(rvconstructor->ConstructR2(RISCV_FMV_W_X, r_src, temp_int));
    }    
    else if (src->GetOperandType() == BasicOperand::REG)
        r_src = GetReg(((RegOperand*)src)->GetRegNo(),FLOAT64);
    cur_block->push_back(rvconstructor->ConstructR2(RISCV_FCVT_W_S, r_dst, r_src));
}

template <> void RiscV64Selector::ConvertAndAppend<SitofpInstruction *>(SitofpInstruction *ins) {
    //TODO("Implement this if you need");
    auto src = ins->GetSrc();
    int dstno = ((RegOperand*)ins->GetResult())->GetRegNo();
    Register r_src,r_dst;
    r_dst = GetReg(dstno, FLOAT64);
    if (src->GetOperandType() == BasicOperand::IMMI32) {
        int val = ((ImmI32Operand *)src)->GetIntImmVal();
        r_src = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
        cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, r_src, GetPhysicalReg(RISCV_x0),val));
    }
    else if (src->GetOperandType() == BasicOperand::REG)
        r_src = GetReg(((RegOperand*)src)->GetRegNo(),INT64);
    cur_block->push_back(rvconstructor->ConstructR2(RISCV_FCVT_S_W, r_dst, r_src));
}

template <> void RiscV64Selector::ConvertAndAppend<ZextInstruction *>(ZextInstruction *ins) {
    //TODO("Implement this if you need");
    auto r_src = GetReg(((RegOperand *)ins->GetSrc())->GetRegNo(), INT64);
    auto r_res = GetReg(((RegOperand *)ins->GetResult())->GetRegNo(), INT64);
    cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, r_res, r_src, 0));
}

template <> void RiscV64Selector::ConvertAndAppend<GetElementptrInstruction *>(GetElementptrInstruction *ins) {
    //TODO("Implement this if you need");
    int resultno = ((RegOperand *)ins->GetResult())->GetRegNo();
    int p = 4;
    for (auto size : ins->GetDims())
        p *= size;
    Register r_offset = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
    Register r_result = GetReg(resultno, INT64);
    cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, r_offset, GetPhysicalReg(RISCV_x0),0));
    for (int i = 0; i < ins->GetIndexes().size(); i++) {
        auto idx = ins->GetIndexes()[i];
        auto type = idx->GetOperandType();
        if(type == BasicOperand::IMMI32)
            cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, r_offset, r_offset, (((ImmI32Operand *)idx)->GetIntImmVal()) * p));
        else{
            int idxno = ((RegOperand *)ins->GetIndexes()[i])->GetRegNo();
            auto r_idx = GetReg(idxno, INT64);
            auto r_p = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
            auto r_set = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
            cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDIW, r_p, GetPhysicalReg(RISCV_x0), p));
            cur_block->push_back(rvconstructor->ConstructR(RISCV_MULW, r_set, r_idx, r_p));
            cur_block->push_back(rvconstructor->ConstructR(RISCV_ADDW, r_offset, r_offset, r_set));
        }
        if (i < ins->GetDims().size())
            p /= ins->GetDims()[i];
    }
    auto base = ins->GetPtrVal();
    if (base->GetOperandType() == BasicOperand::REG) {
        auto baseno = ((RegOperand *)base)->GetRegNo();
        if (alloca_table.find(baseno) == alloca_table.end()) {
            Register r_base = GetReg(baseno, INT64);
            cur_block->push_back(rvconstructor->ConstructR(RISCV_ADD, r_result, r_base, r_offset));
        }
        else{
            cur_block->push_back(rvconstructor->ConstructIImm(RISCV_ADDI,r_result, GetPhysicalReg(RISCV_sp), alloca_table[baseno]));
            cur_block->push_back(rvconstructor->ConstructR(RISCV_ADD, r_result, r_result, r_offset));
        }
    }
    else if (base->GetOperandType() == BasicOperand::GLOBAL){
        auto name = ((GlobalOperand *)(ins->GetPtrVal()))->GetName();
        Register tmp = cur_func->GetNewRegister(INT64.data_type, INT64.data_length);
        cur_block->push_back(rvconstructor->ConstructULabel(RISCV_LUI, tmp, RiscVLabel(name, true)));
        cur_block->push_back(rvconstructor->ConstructILabel(RISCV_ADDI, tmp, tmp, RiscVLabel(name, false)));
        cur_block->push_back(rvconstructor->ConstructR(RISCV_ADD, r_result, tmp, r_offset));
    }
}

template <> void RiscV64Selector::ConvertAndAppend<PhiInstruction *>(PhiInstruction *ins) {
    //TODO("Implement this if you need");
    auto res = (RegOperand *)ins->GetResult();
    if (ins->GetDataType() == BasicInstruction::I32 || ins->GetDataType() == BasicInstruction::PTR){
        Register r_result = GetReg(res->GetRegNo(), INT64);
        int r_no = r_result.reg_no;
        //auto phi = new MachinePhiInstruction(r_result);
        int min_no = r_no;
        if(phi_table.find(r_no) != phi_table.end()){
            if(min_no > phi_table[r_no])
                min_no = phi_table[r_no];
        }
        for (auto [label, op] : ins->phi_list) {
            int labelno = ((LabelOperand *)label)->GetLabelNo();
            int opno = ((RegOperand*)op)->GetRegNo();
            Register r_op = GetReg(opno, INT64);
            int r_op_no = r_op.reg_no;
            if(min_no > r_op_no)
                min_no = r_op_no;
            if(phi_table.find(r_op_no) != phi_table.end()){
                if(min_no > phi_table[r_op_no])
                    min_no = phi_table[r_op_no];
            }
        //    phi->pushPhiList(labelno,r_op);
        }
        phi_table[r_no] = min_no;
        for (auto [label, op] : ins->phi_list) {
            int opno = ((RegOperand*)op)->GetRegNo();
            Register r_op = GetReg(opno, INT64);
            int r_op_no = r_op.reg_no;
            phi_table[r_op_no] = min_no;
        }
        //cur_block->push_back(phi);
        
    }
    else if(ins->GetDataType() == BasicInstruction::FLOAT32){
        Register r_result = GetReg(res->GetRegNo(), FLOAT64);
        int r_no = r_result.reg_no;
        //auto phi = new MachinePhiInstruction(r_result);
        int min_no = r_no;
        for (auto [label, op] : ins->phi_list) {
            int labelno = ((LabelOperand *)label)->GetLabelNo();
            int opno = ((RegOperand*)op)->GetRegNo();
            auto r_op = GetReg(opno, FLOAT64);
            int r_op_no = r_op.reg_no;
            if(min_no > r_op_no)
                min_no = r_op_no;
            //phi->pushPhiList(labelno,r_op);
        }
        phi_table[r_no] = min_no;
        for (auto [label, op] : ins->phi_list) {
            int opno = ((RegOperand*)op)->GetRegNo();
            Register r_op = GetReg(opno, FLOAT64);
            int r_op_no = r_op.reg_no;
            phi_table[r_op_no] = min_no;
        }
        //cur_block->push_back(phi);
    }
}

template <> void RiscV64Selector::ConvertAndAppend<Instruction>(Instruction inst) {
    switch (inst->GetOpcode()) {
    case BasicInstruction::LOAD:
        ConvertAndAppend<LoadInstruction *>((LoadInstruction *)inst);
        break;
    case BasicInstruction::STORE:
        ConvertAndAppend<StoreInstruction *>((StoreInstruction *)inst);
        break;
    case BasicInstruction::ADD:
    case BasicInstruction::SUB:
    case BasicInstruction::MUL:
    case BasicInstruction::DIV:
    case BasicInstruction::FADD:
    case BasicInstruction::FSUB:
    case BasicInstruction::FMUL:
    case BasicInstruction::FDIV:
    case BasicInstruction::MOD:
    case BasicInstruction::SHL:
    case BasicInstruction::BITXOR:
        ConvertAndAppend<ArithmeticInstruction *>((ArithmeticInstruction *)inst);
        break;
    case BasicInstruction::ICMP:
        ConvertAndAppend<IcmpInstruction *>((IcmpInstruction *)inst);
        break;
    case BasicInstruction::FCMP:
        ConvertAndAppend<FcmpInstruction *>((FcmpInstruction *)inst);
        break;
    case BasicInstruction::ALLOCA:
        ConvertAndAppend<AllocaInstruction *>((AllocaInstruction *)inst);
        break;
    case BasicInstruction::BR_COND:
        ConvertAndAppend<BrCondInstruction *>((BrCondInstruction *)inst);
        break;
    case BasicInstruction::BR_UNCOND:
        ConvertAndAppend<BrUncondInstruction *>((BrUncondInstruction *)inst);
        break;
    case BasicInstruction::RET:
        ConvertAndAppend<RetInstruction *>((RetInstruction *)inst);
        break;
    case BasicInstruction::ZEXT:
        ConvertAndAppend<ZextInstruction *>((ZextInstruction *)inst);
        break;
    case BasicInstruction::FPTOSI:
        ConvertAndAppend<FptosiInstruction *>((FptosiInstruction *)inst);
        break;
    case BasicInstruction::SITOFP:
        ConvertAndAppend<SitofpInstruction *>((SitofpInstruction *)inst);
        break;
    case BasicInstruction::GETELEMENTPTR:
        ConvertAndAppend<GetElementptrInstruction *>((GetElementptrInstruction *)inst);
        break;
    case BasicInstruction::CALL:
        ConvertAndAppend<CallInstruction *>((CallInstruction *)inst);
        break;
    case BasicInstruction::PHI:
        ConvertAndAppend<PhiInstruction *>((PhiInstruction *)inst);
        break;
    default:
        ERROR("Unknown LLVM IR instruction");
    }
}

void RiscV64Selector::SelectInstructionAndBuildCFG() {
    // 与中间代码生成一样, 如果你完全无从下手, 可以先看看输出是怎么写的
    // 即riscv64gc/instruction_print/*  common/machine_passes/machine_printer.h

    // 指令选择除了一些函数调用约定必须遵守的情况需要物理寄存器，其余情况必须均为虚拟寄存器
    dest->global_def = IR->global_def;
    // 遍历每个LLVM IR函数
    for (auto [defI,cfg] : IR->llvm_cfg) {
        if(cfg == nullptr){
            ERROR("LLVMIR CFG is Empty, you should implement BuildCFG in MidEnd first");
        }
        std::string name = cfg->function_def->GetFunctionName();

        cur_func = new RiscV64Function(name);
        cur_func->SetParent(dest);
        // 你可以使用cur_func->GetNewRegister来获取新的虚拟寄存器
        dest->functions.push_back(cur_func);

        auto cur_mcfg = new MachineCFG;
        cur_func->SetMachineCFG(cur_mcfg);

        // 清空指令选择状态(可能需要自行添加初始化操作)
        ClearFunctionSelectState();

        // TODO: 添加函数参数(推荐先阅读一下riscv64_lowerframe.cc中的代码和注释)
        // See MachineFunction::AddParameter()
        //TODO("Add function parameter if you need");
        int param_offset = 0;
        for (int i = 0; i < (defI->formals).size(); i++) {
            MachineDataType type;
            if (defI->formals[i] == BasicInstruction::FLOAT32)
                type = FLOAT64;
            else
                type = INT64;
            param_offset += 8;
            cur_func->AddParameter(GetReg(((RegOperand *)defI->formals_reg[i])->GetRegNo(), type));
        }
        
        int call_param_offset = 0;

        for (auto [id, block] : *(cfg->block_map)) {
            for (auto ins : block->Instruction_list) {
                if(ins->GetOpcode()==BasicInstruction::CALL){
                    int now_param_offset = 8 * (((CallInstruction*)ins)->GetParameterList()).size();
                    call_param_offset = (call_param_offset >= now_param_offset) ? call_param_offset : now_param_offset;
                }
            }
        }

        cur_offset = call_param_offset;

        // 遍历每个LLVM IR基本块
        for (auto [id, block] : *(cfg->block_map)) {
            cur_block = new RiscV64Block(id);
            // 将新块添加到Machine CFG中
            cur_mcfg->AssignEmptyNode(id, cur_block);
            cur_func->UpdateMaxLabel(id);

            cur_block->setParent(cur_func);
            cur_func->blocks.push_back(cur_block);

            // 指令选择主要函数, 请注意指令选择时需要维护变量cur_offset
            for (auto instruction : block->Instruction_list) {
                // Log("Selecting Instruction");
                ConvertAndAppend<Instruction>(instruction);
            }
        }

        // RISCV 8字节对齐（）
        if (cur_offset % 8 != 0) {
            cur_offset = ((cur_offset + 7) / 8) * 8;
        }

        // 控制流图连边
        for (int i = 0; i < cfg->G.size(); i++) {
            const auto &arcs = cfg->G[i];
            for (auto arc : arcs) {
                cur_mcfg->MakeEdge(i, arc->block_id);
            }
        }

        for (auto &b : cur_func->blocks) {
            cur_block = b;
            if (b->getLabelId() == 0) {    // 函数入口，需要插入获取参数的指令
                int offset = 0;
                for (auto para : cur_func->GetParameters()) {    // 你需要在指令选择阶段正确设置parameters的值
                    if (para.type.data_type == INT64.data_type) {
                        //if (i32_cnt < 8) {    // 插入使用寄存器传参的指令
                        //    b->push_front(rvconstructor->ConstructR(RISCV_ADD, para, GetPhysicalReg(RISCV_a0 + i32_cnt),
                        //                                            GetPhysicalReg(RISCV_x0)));
                        //}
                        //if (i32_cnt >= 8) {    // 插入使用内存传参的指令
                            //TODO("Implement this if you need");
                        //}
                        
                        b->push_front(rvconstructor->ConstructIImm(RISCV_LD, para, GetPhysicalReg(RISCV_s0), offset));
                        offset += 8;
                    } else if (para.type.data_type == FLOAT64.data_type) {    // 处理浮点数
                        //TODO("Implement this if you need");
                        b->push_front(rvconstructor->ConstructIImm(RISCV_FLW, para, GetPhysicalReg(RISCV_s0), offset));
                        offset += 8;
                    } else {
                        ERROR("Unknown type");
                    }
                }
            }
        }

        //for(auto [s,f]:phi_table){
        //    std::cout<<'p'<<s<<' '<<f<<'\n';
        //}

        for (auto &b : cur_func->blocks) {//虚拟寄存器转物理寄存器
            cur_block = b;
            auto ins = &(b->instructions);
            std::list<MachineBaseInstruction*> newlist;
            for (auto it = ins->begin(); it != ins->end(); ++it) {
                auto inst = *it;
                if(((RiscV64Instruction*)inst)->getOpcode() == RISCV_CALL){
                    newlist.push_back(inst);
                    continue;
                }
                auto readreg = inst->GetReadReg();
                int iregcnt = 0, fregcnt = 0;
                for(auto reg : readreg){
                    if(!(reg->is_virtual))
                        continue;
                    int regno = reg -> reg_no;
                    int parent = GetPhiParent(regno);
                    int off = GetReallocaOffset(parent);
                    //std::cout<<regno<<' '<<parent<<' '<<off<<'\n';
                    if(reg->type == INT64){
                        newlist.push_back(rvconstructor->ConstructIImm(RISCV_LD, GetPhysicalReg(RISCV_a6 - iregcnt), GetPhysicalReg(RISCV_sp), off));
                        *reg = GetPhysicalReg(RISCV_a6 - iregcnt);
                        iregcnt++;
                    }
                    else if(reg->type == FLOAT64){
                        newlist.push_back(rvconstructor->ConstructIImm(RISCV_FLW, GetPhysicalReg(RISCV_fa6 - fregcnt), GetPhysicalReg(RISCV_sp), off));
                        *reg = GetPhysicalReg(RISCV_fa6 - fregcnt);
                        fregcnt++;
                    }
                }
                auto writereg = inst->GetWriteReg();
                bool isadd = false;
                for(auto reg: writereg){ //at most 1 time
                    if(!(reg->is_virtual))
                        continue;
                    int regno = reg -> reg_no;
                    int parent = GetPhiParent(regno);
                    int off = GetReallocaOffset(parent);
                    //std::cout<<regno<<' '<<parent<<' '<<off<<'\n';
                    if(reg->type == INT64){
                        *reg = GetPhysicalReg(RISCV_a6 - iregcnt);
                        newlist.push_back(inst);
                        isadd = true;
                        newlist.push_back(rvconstructor->ConstructSImm(RISCV_SD, GetPhysicalReg(RISCV_a6 - iregcnt), GetPhysicalReg(RISCV_sp), off));
                        iregcnt++;
                    }
                    else if(reg->type == FLOAT64){
                        *reg = GetPhysicalReg(RISCV_fa6 - fregcnt);
                        newlist.push_back(inst);
                        isadd = true;
                        newlist.push_back(rvconstructor->ConstructSImm(RISCV_FSW, GetPhysicalReg(RISCV_fa6 - fregcnt), GetPhysicalReg(RISCV_sp), off));
                        fregcnt++;
                    }
                }
                if(!isadd)
                    newlist.push_back(inst);
            }
            b->instructions = newlist;
        }
        cur_func->SetStackSize(cur_offset + param_offset + 16);

        for (auto &b : cur_func->blocks) {//增加栈操作
            cur_block = b;
            if (b->getLabelId() == 0) {
                cur_block->push_front(rvconstructor->ConstructIImm(RISCV_ADDI, GetPhysicalReg(RISCV_s0), GetPhysicalReg(RISCV_sp), cur_offset + param_offset + 16));
                cur_block->push_front(rvconstructor->ConstructSImm(RISCV_SD, GetPhysicalReg(RISCV_s0), GetPhysicalReg(RISCV_sp), cur_offset + param_offset));
                cur_block->push_front(rvconstructor->ConstructSImm(RISCV_SD, GetPhysicalReg(RISCV_ra), GetPhysicalReg(RISCV_sp), cur_offset + param_offset + 8));
                cur_block->push_front(rvconstructor->ConstructIImm(RISCV_ADDI, GetPhysicalReg(RISCV_sp), GetPhysicalReg(RISCV_sp), - cur_offset - param_offset - 16));
            }
        }

        for (auto &b : cur_func->blocks) {//处理大立即数
            cur_block = b;
            auto ins = &(b->instructions);
            std::list<MachineBaseInstruction*> newlist;
            for (auto it = ins->begin(); it != ins->end(); ++it) {
                auto inst = (RiscV64Instruction*)*it;
                int imm;
                switch (inst->getOpcode())
                {
                    case RISCV_ADDI:
                        imm = inst->getImm();
                        if(imm >= (1<<11) || imm <= -(1<<11)){
                            auto rs1 = inst->getRs1();
                            auto rd = inst->getRd();
                            newlist.push_back(rvconstructor->ConstructUImm(RISCV_LI, GetPhysicalReg(RISCV_t1), imm));
                            newlist.push_back(rvconstructor->ConstructR(RISCV_ADD, rd, rs1, GetPhysicalReg(RISCV_t1)));
                        }
                        else
                            newlist.push_back(inst);
                        break;
                    case RISCV_ADDIW:
                        imm = inst->getImm();
                        if(imm >= (1<<11) || imm <= -(1<<11)){
                            auto rs1 = inst->getRs1();
                            auto rd = inst->getRd();
                            newlist.push_back(rvconstructor->ConstructUImm(RISCV_LI, GetPhysicalReg(RISCV_t1), imm));
                            newlist.push_back(rvconstructor->ConstructR(RISCV_ADDW, rd, rs1, GetPhysicalReg(RISCV_t1)));
                        }
                        else
                            newlist.push_back(inst);
                        break;
                    case RISCV_LW:
                        imm = inst->getImm();
                        if(imm >= (1<<11) || imm <= -(1<<11)){
                            auto rs1 = inst->getRs1();
                            auto rd = inst->getRd();
                            newlist.push_back(rvconstructor->ConstructUImm(RISCV_LI, GetPhysicalReg(RISCV_t1), imm));
                            newlist.push_back(rvconstructor->ConstructR(RISCV_ADD, GetPhysicalReg(RISCV_t1), rs1, GetPhysicalReg(RISCV_t1)));
                            newlist.push_back(rvconstructor->ConstructIImm(RISCV_LW, rd, GetPhysicalReg(RISCV_t1), 0));
                        }
                        else
                            newlist.push_back(inst);
                        break;
                    case RISCV_SW:
                        imm = inst->getImm();
                        if(imm >= (1<<11) || imm <= -(1<<11)){
                            auto rs1 = inst->getRs1();
                            auto rs2 = inst->getRs2();
                            newlist.push_back(rvconstructor->ConstructUImm(RISCV_LI, GetPhysicalReg(RISCV_t1), imm));
                            newlist.push_back(rvconstructor->ConstructR(RISCV_ADD, GetPhysicalReg(RISCV_t1), rs2, GetPhysicalReg(RISCV_t1)));
                            newlist.push_back(rvconstructor->ConstructSImm(RISCV_SW, rs1, GetPhysicalReg(RISCV_t1), 0));
                        }
                        else
                            newlist.push_back(inst);
                        break;
                    case RISCV_FLW:
                        imm = inst->getImm();
                        if(imm >= (1<<11) || imm <= -(1<<11)){
                            auto rs1 = inst->getRs1();
                            auto rd = inst->getRd();
                            newlist.push_back(rvconstructor->ConstructUImm(RISCV_LI, GetPhysicalReg(RISCV_t1), imm));
                            newlist.push_back(rvconstructor->ConstructR(RISCV_ADD, GetPhysicalReg(RISCV_t1), rs1, GetPhysicalReg(RISCV_t1)));
                            newlist.push_back(rvconstructor->ConstructIImm(RISCV_FLW, rd, GetPhysicalReg(RISCV_t1), 0));
                        }
                        else
                            newlist.push_back(inst);
                        break;
                    case RISCV_FSW:
                        imm = inst->getImm();
                        if(imm >= (1<<11) || imm <= -(1<<11)){
                            auto rs1 = inst->getRs1();
                            auto rs2 = inst->getRs2();
                            newlist.push_back(rvconstructor->ConstructUImm(RISCV_LI, GetPhysicalReg(RISCV_t1), imm));
                            newlist.push_back(rvconstructor->ConstructR(RISCV_ADD, GetPhysicalReg(RISCV_t1), rs2, GetPhysicalReg(RISCV_t1)));
                            newlist.push_back(rvconstructor->ConstructSImm(RISCV_FSW, rs1, GetPhysicalReg(RISCV_t1), 0));
                        }
                        else
                            newlist.push_back(inst);
                        break;
                    case RISCV_LD:
                        imm = inst->getImm();
                        if(imm >= (1<<11) || imm <= -(1<<11)){
                            auto rs1 = inst->getRs1();
                            auto rd = inst->getRd();
                            newlist.push_back(rvconstructor->ConstructUImm(RISCV_LI, GetPhysicalReg(RISCV_t1), imm));
                            newlist.push_back(rvconstructor->ConstructR(RISCV_ADD, GetPhysicalReg(RISCV_t1), rs1, GetPhysicalReg(RISCV_t1)));
                            newlist.push_back(rvconstructor->ConstructIImm(RISCV_LD, rd, GetPhysicalReg(RISCV_t1), 0));
                        }
                        else
                            newlist.push_back(inst);
                        break;
                    case RISCV_SD:
                        imm = inst->getImm();
                        if(imm >= (1<<11) || imm <= -(1<<11)){
                            auto rs1 = inst->getRs1();
                            auto rs2 = inst->getRs2();
                            newlist.push_back(rvconstructor->ConstructUImm(RISCV_LI, GetPhysicalReg(RISCV_t1), imm));
                            newlist.push_back(rvconstructor->ConstructR(RISCV_ADD, GetPhysicalReg(RISCV_t1), rs2, GetPhysicalReg(RISCV_t1)));
                            newlist.push_back(rvconstructor->ConstructSImm(RISCV_SD, rs1, GetPhysicalReg(RISCV_t1), 0));
                        }
                        else
                            newlist.push_back(inst);
                        break;
                    default:
                        newlist.push_back(inst);
                        break;
                }
            }
            b->instructions = newlist;
        }
    }
}

int RiscV64Selector::GetReallocaOffset(int parent){
    if(realloca_table.find(parent) == realloca_table.end()){
        realloca_table[parent] = cur_offset;
        cur_offset += 8;
    }
    return realloca_table[parent];
}

int RiscV64Selector::GetPhiParent(int child){
    if(phi_table.find(child) == phi_table.end())
        phi_table[child] = child;
    else if(phi_table[child] != child)
        phi_table[child] = GetPhiParent(phi_table[child]);
    return phi_table[child];
}

void RiscV64Selector::ClearFunctionSelectState() { 
    cur_offset = 0;
    alloca_table.clear();
    register_table.clear();
    phi_table.clear();
    realloca_table.clear();
}
