#ifndef __BOO_VM_H__
#define __BOO_VM_H__

namespace boovm {
    #define BOO_EXIT 1

    enum {
        OP_EXIT = 0,
        OP_LOADC,
        OP_LOAD,
        OP_STORE,
        OP_ADD,
        OP_MUL,
        OP_CALL
    };

    struct Value {
            const char *str;
            int iv;
    };

    struct Instruction {
        char op;
        Value oprand;
    };

    class Bytecode {
        public:
            Bytecode();
            ~Bytecode();

            void Push(Instruction code);
            unsigned *GetPC() {
                return &len_;
            }
            unsigned GetLength() {
                return len_;
            }

            Instruction operator[](int idx) {
                return inst_[idx];
            }

        private:
            Instruction *inst_;
            unsigned cap_;
            unsigned len_;
    };

    class Stack {
        public:
            Stack();
            ~Stack();

            void Push(Value v);
            void Pop();

            int *GetSP() {
                return &SP_;
            }
            
            Value operator[](int idx) {
                return s_[idx];
            }

        private:
            Value *s_;
            unsigned cap_;
            unsigned len_;
            int SP_;
    };

    class Memory {
        public:
            Memory();
            ~Memory();

            char *operator[](int idx) {
                return mem_ + idx;
            }

        private:
            char *mem_;
            unsigned cap_;
            unsigned len_;
    };

    class VM {
        public:
            VM();
            ~VM();

            void Compile(const char *asm_code, unsigned len);
            void Run();

            Value GetResult() {
                return stack_[*SP_];
            }

            Memory &GetMemory() {
                return mem_;
            }

        private:
            Bytecode bytecode_;
            Stack stack_;
            Memory mem_;
            int *SP_;
            unsigned PC_;

            int execute(Instruction inst);
    };

};

#endif