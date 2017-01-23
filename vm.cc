#include "vm.h"

#include <stdlib.h>
#include <stdio.h>

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
    PC_ = 0;
    SP_ = stack_.GetSP();
}
VM::~VM() {
    
}

void VM::Compile(const char *asm_code, unsigned len) {
    std::string op;
    for (unsigned i=0; i<len; ++i) {
        op += asm_code[i];
        //printf("%s", op.c_str());
        //fflush(0);
        if (op == "loadc") {
            i += 2;

            std::string str_oprand;
            while (asm_code[i] != '\n'&&i<len) {
                str_oprand += asm_code[i++];
            }
            Value oprand{nullptr, 0};
            oprand.iv = atoi(str_oprand.c_str());
            Bytecode_ c{OP_LOADC, oprand};
            bytecode_.Push(c);
            op = "";
        } else if (op == "add") {
            Value oprand{nullptr, 0};
            Bytecode_ c{OP_ADD, oprand};
            bytecode_.Push(c);
            op = "";
        } else if (op == "callc") {
            i += 2;

            std::string str_oprand;
            while (asm_code[i] != '\n'&&i<len) {
                str_oprand += asm_code[i++];
            }
            fflush(0);
            if (str_oprand == "print") {
                Value oprand{"print", 0};
                Bytecode_ c{OP_CALLC, oprand};
                bytecode_.Push(c);
            }
            op = "";
        } else if (op == "\n") {
            op = "";
        }
    }

     Bytecode_ c{OP_HALT, 0};
     bytecode_.Push(c);
}
void VM::Run() {
    for (;;) {
        Bytecode_ code = bytecode_[PC_++];
        execute(code);
    }
}

void VM::execute(Bytecode_ code) {
    Value oprand1{nullptr, 0};
    Value oprand2{nullptr, 0};
    Value result{nullptr, 0};
    //printf("\n%d\n", code.op);
    //fflush(0);
    switch (code.op) {
        case OP_LOADC:
        stack_.Push(code.oprand);
        break;

        case OP_ADD:
        oprand1 = stack_[(*SP_)-1];
        //printf("%d %d", *SP_, oprand1.iv);
        //fflush(0);
        oprand2 = stack_[(*SP_)-2];
        result.iv = oprand1.iv + oprand2.iv;
        stack_.Push(result);
        break;

        case OP_CALLC:
        oprand1 = stack_[(*SP_)-1];
        if (strcmp(code.oprand.str, "print") == 0) {
            printf("%d", oprand1.iv);
            fflush(0);
        }
        break;

        case OP_HALT:
        while(1) {}
        break;
    }
}

int main(int argc, char *argv[]) {
    const char asm_code[] = "loadc 1\n"
                            "loadc 2\n"
                            "add\n"
                            "callc print";

    VM vm;
    vm.Compile(asm_code, strlen(asm_code));
    vm.Run();
    return 0;
}