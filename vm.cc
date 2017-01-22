#include "vm.h"

#include <stdlib.h>
#include <string>

using namespace boovm;

Bytecode::Bytecode() : bytecode_{nullptr}, len_{1000}, idx_{0}
{
    bytecode_ = new Bytecode_[len_];
}
Bytecode::~Bytecode() {
    delete [] bytecode_;
}

void Bytecode::Push(Bytecode_ code) {
    bytecode_[idx_++] = code;
}

Stack::Stack() : s_{nullptr}, len_{10}, idx_{0} {
    s_ = new Value[len_];
}
Stack::~Stack() {
    delete [] s_;
}

void Stack::Push(Value v) {
    s_[idx_++] = v;
}
Value Stack::Pop() {
    return s_[idx_--];
}

VM::VM() {
    PC_ = bytecode_.GetPC();
    SP_ = stack_.GetSP();
}
VM::~VM() {
    
}

void VM::Compile(const char *asm_code, unsigned len) {
    for (unsigned i=0; i<len; ++i) {
        std::string op;
        op += asm_code[i];
        if (op == "loadc") {
            ++i;
            std::string str_oprand;
            while (asm_code[i] != '\n'||i<len) {
                str_oprand += asm_code[i++];
            }
            Value oprand = atoi(str_oprand.c_str());
            Bytecode_ c{OP_LOADC, oprand};
            bytecode_.Push(c);
            op = "";
        } else if (op == "add") {
            Bytecode_ c{OP_ADD, 0};
            bytecode_.Push(c);
            op = "";
        } else if (op == "\n") {

        }
    }

     Bytecode_ c{OP_HALT, 0};
     bytecode_.Push(c);
}
void VM::Run() {
    for (;;) {
        Bytecode_ code = bytecode_[(*PC_)++];
        execute(code);
    }

}

void VM::execute(Bytecode_ code) {
    Value oprand1;
    Value oprand2;
    Value result;
    switch (code.op) {
        case OP_LOADC:
        stack_.Push(code.oprand);
        break;

        case OP_ADD:
        oprand1 = stack_[(*SP_)-1];
        oprand2 = stack_[(*SP_)-2];
        result = oprand1 + oprand2;
        stack_.Push(result);
        break;

        case OP_HALT:
        while(1) {}
        break;
    }
}

int main(int argc, char *argv[]) {
    const char asm_code[] = "loadc 1\n"
                            "loadc 2\n"
                            "add";

    VM vm;
    vm.Compile(asm_code, strlen(asm_code));
    vm.Run();
    return 0;
}