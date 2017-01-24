#include "vm.h"

int main(int argc, char *argv[]) {
    const char asm_code[] = "loadc 1\n"
                            "loadc 2\n"
                            "add\n"
                            "call print";

    VM vm;
    vm.Compile(asm_code, strlen(asm_code));
    vm.Run();
    return 0;
}