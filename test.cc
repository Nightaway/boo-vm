#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"

#define PRINT_PASS printf("===%20s Num %d of %d PASS OK===\n",test_list[num-1].name, num, len);
#define PRINT_FAIL printf("===%20s Num %d of %d FAIL OK===\n",test_list[num-1].name, num, len);

typedef int (*TestFunc)(const char *code, long size);
struct TestList {
    const char *name;
    TestFunc func;
};

int func_add1(const char *code, long size);
int func_add2(const char *code, long size);
int func_load1(const char *code, long size);
int func_store1(const char *code, long size);
int func_jump1(const char *code, long size);
int func_sub1(const char *code, long size);
int func_less1(const char *code, long size);
int func_equal1(const char *code, long size);
int func_test1(const char *code, long size);

TestList test_list[] = {
        {"add1.asm", func_add1},
        {"add2.asm", func_add2},
        {"load1.asm", func_load1},
        {"store1.asm", func_store1},
        {"jump1.asm", func_jump1},
        {"less1.asm", func_less1},
        {"sub1.asm", func_sub1},
        {"equal1.asm", func_equal1},
        {"test1.asm", func_test1}
};

static int len = sizeof(test_list)/sizeof(test_list[0]);
static int num = 1;

void Expect(boovm::Value v, boovm::Value expect) {
    if (v.str != nullptr&&expect.str != nullptr) {
        if (strcmp(v.str, expect.str)==0 && v.iv == expect.iv) {
                PRINT_PASS;
                num++;
        } else {
            PRINT_FAIL;
            num++;
        }
    } else if (v.str == nullptr&&expect.str == nullptr) {
        if (v.iv == expect.iv) {
                PRINT_PASS;
                num++;
        } else {
            PRINT_FAIL;
            num++;
        }
    } else {
         PRINT_FAIL;
         num++;
    }

}

int func_add1(const char *code, long size) {
        boovm::VM vm;
        vm.Compile(code, size);
        vm.Run();
        boovm::Value expect{nullptr, 3};
        boovm::Value v = vm.GetResult();
        Expect(v, expect);
        return 0;
}

int func_add2(const char *code, long size) {
        boovm::VM vm;
        vm.Compile(code, size);
        vm.Run();
        boovm::Value expect{nullptr, 70};
        boovm::Value v = vm.GetResult();
        Expect(v, expect);
        return 0;
}

int func_load1(const char *code, long size) {
        boovm::VM vm;
        boovm::Memory &mem = vm.GetMemory();
        boovm::Value *i = (boovm::Value *)mem[0];
        i->str = nullptr;
        i->iv = 11;
        vm.Compile(code, size);
        vm.Run();
        boovm::Value expect{nullptr, 11};
        boovm::Value v = vm.GetResult();
        Expect(v, expect);
        return 0;
}

int func_store1(const char *code, long size) {
        boovm::VM vm;
        vm.Compile(code, size);
        vm.Run();
        boovm::Memory &mem = vm.GetMemory();
        boovm::Value *v = (boovm::Value *)mem[0];
        boovm::Value expect{nullptr, 1};
        Expect(*v, expect);
        return 0;
}

int func_jump1(const char *code, long size) {
        boovm::VM vm;
        vm.Compile(code, size);
        vm.Run();
        boovm::Value expect{nullptr, 0};
        boovm::Value v = vm.GetResult();
        Expect(v, expect);
        return 0;
}

int func_less1(const char *code, long size) {
        boovm::VM vm;
        vm.Compile(code, size);
        vm.Run();
        boovm::Value expect{nullptr, 0};
        boovm::Value v = vm.GetResult();
        Expect(v, expect);
        return 0;
}

int func_sub1(const char *code, long size) {
        boovm::VM vm;
        vm.Compile(code, size);
        vm.Run();
        boovm::Value expect{nullptr, -1};
        boovm::Value v = vm.GetResult();
        Expect(v, expect);
        return 0;
}

int func_equal1(const char *code, long size) {
        boovm::VM vm;
        vm.Compile(code, size);
        vm.Run();
        boovm::Value expect{nullptr, 0};
        boovm::Value v = vm.GetResult();
        Expect(v, expect);
        return 0;
}

int func_test1(const char *code, long size) {
        boovm::VM vm;
        vm.Compile(code, size);
        vm.Run();
        boovm::Value expect{nullptr, 2};
        boovm::Value v = vm.GetResult();
        Expect(v, expect);
        return 0;
}

int main(int argc, char *argv[]) {

    for (int i=0; i<sizeof(test_list)/sizeof(test_list[0]); ++i) {
        FILE *f = fopen(test_list[i].name, "r");
        if (f == NULL) {
            continue;
        }
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        char *bytes = (char *)malloc(sizeof(char) * size + 1);
        rewind(f);
        fread(bytes, size, 1, f);
        test_list[i].func(bytes, size);
        fclose(f);
        free(bytes);
    }
    
    return 0;
}