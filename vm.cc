#include "vm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>

using namespace boovm;

Bytecode::Bytecode() : inst_{nullptr}, cap_{1000}, len_{0}
{
    inst_ = new Instruction[cap_];
}
Bytecode::~Bytecode() {
    delete [] inst_;
}

void Bytecode::Push(Instruction inst) {
    inst_[len_++] = inst;
}

Stack::Stack() : s_{nullptr}, cap_{10}, len_{0} {
    s_ = new Value[cap_];
}
Stack::~Stack() {
    delete [] s_;
}

void Stack::Push(Value v) {
    s_[len_++] = v;
}
Value Stack::Pop() {
    return s_[len_--];
}

VM::VM() {
    PC_ = 0;
    SP_ = stack_.GetSP();
}
VM::~VM() {
    
}

void VM::Compile(const char *asm_code, unsigned len) {
    std::string op;
    for (unsigned i=0; i<len; ++i) {
        op += asm_code[i];

        if (op == "loadc") {
            i += 2;

            std::string str_oprand;
            while (asm_code[i] != '\n'&&i<len) {
                str_oprand += asm_code[i++];
            }
            Value oprand{nullptr, 0};
            oprand.iv = atoi(str_oprand.c_str());
            Instruction inst{OP_LOADC, oprand};
            bytecode_.Push(inst);
            op = "";
        } else if (op == "add") {
            Value oprand{nullptr, 0};
            Instruction inst{OP_ADD, oprand};
            bytecode_.Push(inst);
            op = "";
        } else if (op == "call") {
            i += 2;

            std::string str_oprand;
            while (asm_code[i] != '\n'&&i<len) {
                str_oprand += asm_code[i++];
            }
            if (str_oprand == "print") {
                Value oprand{"print", 0};
                Instruction inst{OP_CALLC, oprand};
                bytecode_.Push(inst);
            }
            op = "";
        } else if (op == "\n") {
            op = "";
        }
    }

     Instruction inst{OP_HALT, 0};
     bytecode_.Push(inst);
}
void VM::Run() {
    for (;;) {
        Instruction inst = bytecode_[PC_++];
        int ret = execute(inst);
        if (ret == 1) {
            break;
        }
    }
}

int VM::execute(Instruction inst) {
    Value oprand1{nullptr, 0};
    Value oprand2{nullptr, 0};
    Value result{nullptr, 0};

    switch (inst.op) {
        case OP_LOADC:
        stack_.Push(inst.oprand);
        break;

        case OP_ADD:
        oprand1 = stack_[(*SP_)-1];
        oprand2 = stack_[(*SP_)-2];
        result.iv = oprand1.iv + oprand2.iv;
        stack_.Push(result);
        break;

        case OP_CALLC:
        oprand1 = stack_[(*SP_)-1];
        if (strcmp(inst.oprand.str, "print") == 0) {
            printf("%d", oprand1.iv);
            fflush(0);
        }
        break;

        case OP_HALT:
        return 1;
    }
    return 0;
}

