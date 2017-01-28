#include "vm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <map>
#include <string>

using namespace boovm;

Bytecode::Bytecode() : inst_{nullptr}, cap_{1000}, len_{0} {
    inst_ = new Instruction[cap_];
}
Bytecode::~Bytecode() {
    delete [] inst_;
}

void Bytecode::Push(Instruction inst) {
    inst_[len_++] = inst;
}

Stack::Stack() : s_{nullptr}, cap_{100}, len_{0}, SP_{-1} {
    s_ = new Value[cap_];
}
Stack::~Stack() {
    delete [] s_;
}

void Stack::Push(Value v) {
    s_[++SP_] = v;
    len_++;
}
void Stack::Pop() {
    SP_--;
}


Memory::Memory() : mem_{nullptr}, cap_{10000}, len_{0} {
    mem_ = new char[cap_];
}
Memory::~Memory() {
    delete[] mem_;
}

VM::VM() {
    PC_ = 0;
    SP_ = stack_.GetSP();
}
VM::~VM() {
    
}

std::map<std::string, int> labels;

Instruction makeInst1(const char *asm_code, unsigned &i, int len, char op) {
    std::string str_oprand;
    while (asm_code[i] != '\n'&&i<len) {
        str_oprand += asm_code[i++];
    }
    Value oprand{nullptr, 0};
    oprand.iv = atoi(str_oprand.c_str());
    Instruction inst{op, oprand};
    return inst;
}

Instruction makeJump(const char *asm_code, unsigned &i, int len, char op) {
    std::string str_oprand;
    while (asm_code[i] != '\n'&&i<len) {
        str_oprand += asm_code[i++];
    }
    Value oprand{nullptr, 0};
    oprand.iv = labels[str_oprand];
    Instruction inst{op, oprand};
    return inst;
}


void VM::Compile(const char *asm_code, unsigned len) {
    std::string label;
    int lineno = 0;
    for (unsigned i=0; i<len; ++i) {
        while (asm_code[i] != ':' && asm_code[i] != '\n' && i<len) {
            label += asm_code[i++];
        }

        if (asm_code[i] == ':') {
            labels[label] = lineno;
            //printf("%s %d\n", label.c_str(), lineno);
        } else if (asm_code[i] == '\n') {
            lineno++;
        }
        label = "";
    }

    std::string op;
    for (unsigned i=0; i<len; ++i) {
        while (asm_code[i] != ':' && asm_code[i] != ' ' && asm_code[i] != '\n' && i<len) {
            op += asm_code[i++];
        }

       if (asm_code[i] == ':') {
            ++i;
            op = "";
            continue;
        }
        //printf("%s\n", op.c_str());
            
        if (op == "loadc") {
            i += 1;

            Instruction inst = makeInst1(asm_code, i, len, OP_LOADC);
            bytecode_.Push(inst);
            
        } else if (op == "pop") {
            Value oprand{nullptr, 0};
            Instruction inst{OP_POP, oprand};
            bytecode_.Push(inst);
           
        } else if (op == "load") {
            Value oprand{nullptr, 0};
            Instruction inst{OP_LOAD, oprand};
            bytecode_.Push(inst);
           
        } else if (op == "store") {
            Value oprand{nullptr, 0};
            Instruction inst{OP_STORE, oprand};
            bytecode_.Push(inst);
          
        } else if (op == "add") {
            Value oprand{nullptr, 0};
            Instruction inst{OP_ADD, oprand};
            bytecode_.Push(inst);
            
        } else if (op == "sub") {
            Value oprand{nullptr, 0};
            Instruction inst{OP_SUB, oprand};
            bytecode_.Push(inst);
            
        } else if (op == "mul") {
            Value oprand{nullptr, 0};
            Instruction inst{OP_MUL, oprand};
            bytecode_.Push(inst);
          
        } else if (op == "less") {
            Value oprand{nullptr, 0};
            Instruction inst{OP_LESS, oprand};
            bytecode_.Push(inst);
          
        } else if (op == "equal") {
            Value oprand{nullptr, 0};
            Instruction inst{OP_EQUAL, oprand};
            bytecode_.Push(inst);
          
        } else if (op == "jump") {
            i += 1;

            Instruction inst = makeJump(asm_code, i, len, OP_JUMP);
            bytecode_.Push(inst);
          
        } else if (op == "jumpz") {
            i += 1;

            Instruction inst = makeJump(asm_code, i, len, OP_JUMPZ);
            bytecode_.Push(inst);
          
        } else if (op == "call") {
            i += 1;

            std::string str_oprand;
            while (asm_code[i] != '\n'&&i<len) {
                str_oprand += asm_code[i++];
            }
            if (str_oprand == "print") {
                Value oprand{"print", 0};
                Instruction inst{OP_CALL, oprand};
                bytecode_.Push(inst);
            }
       
        } else if (op == "\n") {
   
        }
        op = "";
    }

     Instruction inst{OP_EXIT, 0};
     bytecode_.Push(inst);
}
void VM::Run() {
    for (;;) {
        Instruction inst = bytecode_[PC_++];
        if (execute(inst) == BOO_EXIT)
            break;
    }
}

int VM::execute(Instruction inst) {
    Value oprand1{nullptr, 0};
    Value oprand2{nullptr, 0};
    Value result{nullptr, 0};
    Value *v{nullptr};

    switch (inst.op) {

        case OP_POP:
        (*SP_)--;
        break;

        case OP_LOADC:
        stack_.Push(inst.oprand);
        break;

        case OP_LOAD:
        oprand1 = stack_[*SP_];
        v = (Value *)mem_[oprand1.iv];
        *SP_ -= 1;
        stack_.Push(*v);
        break;

        case OP_STORE:
        oprand1 = stack_[*SP_];
        oprand2 = stack_[(*SP_)-1];
        v = (Value *)mem_[oprand2.iv];
        v->str = oprand1.str;
        v->iv = oprand1.iv;
        (*SP_)--;
        break;

        case OP_ADD:
        oprand1 = stack_[*SP_];
        oprand2 = stack_[(*SP_)-1];
        result.iv = oprand1.iv + oprand2.iv;
        *SP_ -= 2;
        stack_.Push(result);
        break;

        case OP_SUB:
        oprand1 = stack_[(*SP_)-1];
        oprand2 = stack_[*SP_];
        result.iv = oprand1.iv - oprand2.iv;
        *SP_ -= 2;
        stack_.Push(result);
        break;

        case OP_MUL:
        oprand1 = stack_[*SP_];
        oprand2 = stack_[(*SP_)-1];
        result.iv = oprand1.iv * oprand2.iv;
        *SP_ -= 2;
        stack_.Push(result);
        break;

        case OP_LESS:
        oprand1 = stack_[(*SP_)-1];
        oprand2 = stack_[*SP_];
        result.iv = oprand1.iv < oprand2.iv;
        *SP_ -= 2;
        stack_.Push(result);
        break;

        case OP_EQUAL:
        oprand1 = stack_[*SP_];
        oprand2 = stack_[(*SP_)-1];
        result.iv = oprand1.iv == oprand2.iv;
        *SP_ -= 2;
        stack_.Push(result);
        break;

        case OP_JUMP:
        PC_ = inst.oprand.iv;
        break;

        case OP_JUMPZ:
        oprand1 = stack_[*SP_];
        if (oprand1.iv == 0)
            PC_ = inst.oprand.iv;
        (*SP_)--;
        break;

        case OP_CALL:
        oprand1 = stack_[*SP_];
        if (strcmp(inst.oprand.str, "print") == 0) {
            printf("%d", oprand1.iv);
            fflush(0);
        }
        break;

        case OP_EXIT:
        return BOO_EXIT;
    }
    return 0;
}

