#ifndef RISCV64_INSTSELECT_H
#define RISCV64_INSTSELECT_H
#include "../../common/machine_passes/machine_selector.h"
#include "../riscv64.h"
class RiscV64Selector : public MachineSelector {
private:
    int cur_offset;    // 局部变量在栈中的偏移
    // 你需要保证在每个函数的指令选择结束后, cur_offset的值为局部变量所占栈空间的大小
    
    // TODO(): 添加更多你需要的成员变量和函数
    std::map<int, int> alloca_table;
    std::map<int,Register> register_table;
    std::map<int, int> phi_table;
    std::map<int, int> realloca_table;
public:
    RiscV64Selector(MachineUnit *dest, LLVMIR *IR) : MachineSelector(dest, IR) {}
    void SelectInstructionAndBuildCFG();
    void ClearFunctionSelectState();
    template <class INSPTR> void ConvertAndAppend(INSPTR);
    Register GetReg(int regno, MachineDataType ty);

    int GetPhiParent(int child);
    int GetReallocaOffset(int parent);
};
#endif