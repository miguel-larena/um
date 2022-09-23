#include <stdlib.h>
#include <stdio.h>

int rando(int i) {
    return ((((i ^ 0x12345678) >> ((i % 7) * i)) + i)) % 14;
}

int case0(int v) { return 1 + v; }
int case1(int v) { return 12 + v; }
int case2(int v) { return 22 + v; }
int case3(int v) { return 452 + v; }
int case4(int v) { return 1322 + v; }
int case5(int v) { return 1122 + v; }
int case6(int v) { return 1232 + v; }
int case7(int v) { return 1452 + v; }
int case8(int v) { return 1232 + v; }
int case9(int v) { return 1243 + v; }
int case10(int v) { return 1212 + v; }
int case11(int v) { return 1239 + v; }
int case12(int v) { return 4312 + v; }
int case13(int v) { return 1234 + v; }

typedef int (*Operation)(int);

Operation ops[14] =  {
    &case0, &case1, &case2, &case3, &case4, &case5, &case6, &case7, &case8,
    &case9, &case10, &case11, &case12, &case13
};

int main() {
    int histogram[14] = {0};
    int max = 0x0fffffff;

    for (int i = 0; i < max; i++) {
        int idx = rando(i);

         //switch (idx) {
        // case 0:
        //     histogram[idx] = 1 + histogram[idx];
        //     break;
        // case 1:
        //     histogram[idx] = 12 + histogram[idx];
        //     break;
        // case 2:
        //     histogram[idx] = 22 + histogram[idx];
        //     break;
        // case 3:
        //     histogram[idx] = 452 + (histogram[idx]);
        //     break;
        // case 4:
        //     histogram[idx] = 1322 + (histogram[idx]);
        //     break;
        // case 5:
        //     histogram[idx] = 1122 + (histogram[idx]);
        //     break;
        // case 6:
        //     histogram[idx] = 1232 + (histogram[idx]);
        //     break;
        // case 7:
        //     histogram[idx] = 1452 + (histogram[idx]);
        //     break;
        // case 8:
        //     histogram[idx] = 1232 + (histogram[idx]);
        //     break;
        // case 9:
        //     histogram[idx] = 1243 + (histogram[idx]);
        //     break;
        // case 10:
        //     histogram[idx] = 1212 + (histogram[idx]);
        //     break;
        // case 11:
        //     histogram[idx] = 1239 + (histogram[idx]);
        //     break;
        // case 12:
        //     histogram[idx] = 4312 + (histogram[idx]);
        //     break;
        // case 13:
        //     histogram[idx] = 1234 + (histogram[idx]);
        //     break;
        // }

        histogram[idx] = ops[idx](histogram[idx]);
    }

    for (int i = 0; i < 10; i++)
        fprintf(stderr, " %i: %i\n", i, histogram[i]);

    for (int i = 10; i < 14; i++)
        fprintf(stderr, "%i: %i\n", i, histogram[i]);
}
