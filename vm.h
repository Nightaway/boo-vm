#ifndef __BOO_VM_H__
#define __BOO_VM_H__

namespace boovm {

    enum {
        OP_HALT = 0,
        OP_LOADC,
        OP_ADD,
        OP_CALLC
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
                return &idx_;
            }

            Instruction operator[](int idx) {
                return inst_[idx];
            }

        private:
            Instruction *inst_;
            unsigned len_;
            unsigned idx_;
    };

    class Stack {
        public:
            Stack();
            ~Stack();

            void Push(Value v);
            Value Pop();

            unsigned *GetSP() {
                return &idx_;
            }
            
            Value operator[](int idx) {
                return s_[idx];
            }

        private:
            Value *s_;
            unsigned len_;
            unsigned idx_;
    };

    class VM {
        public:
            VM();
            ~VM();

            void Compile(const char *asm_code, unsigned len);
            void Run();

        private:
            Bytecode bytecode_;
            Stack stack_;
            unsigned *SP_;
            unsigned PC_;

            void execute(Instruction inst);
    };

};

#endif