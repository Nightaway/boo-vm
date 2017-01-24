#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"

typedef int (*TestFunc)(const char *code, long size);
struct TestList {
    const char *name;
    TestFunc func;
};

int func_add1(const char *code, long size);
int func_add2(const char *code, long size);

TestList test_list[] = {
        {"add1.asm", func_add1},
        {"add2.asm", func_add2}
};

static int len = sizeof(test_list)/sizeof(test_list[0]);
static int num = 1;

void Expect(boovm::Value v, boovm::Value expect) {
    if (v.str != nullptr&&expect.str != nullptr) {
        if (strcmp(v.str, expect.str)==0 && v.iv == expect.iv) {
                printf("===Num %d of %d PASS OK===\n", num, len);
                num++;
        } else {
            printf("===Num %d of %d FAIL OK===\n", num, len);
        }
    } else if (v.str == nullptr&&expect.str == nullptr) {
        if (v.iv == expect.iv) {
                printf("===Num %d of %d PASS OK===\n", num, len);
                num++;
        } else {
            printf("===Num %d of %d FAIL OK===\n", num, len);
        }
    } else {
         printf("===Num %d of %d FAIL OK===\n", num, len);
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
        boovm::Value expect{nullptr, 10};
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