#include <stdio.h>
#include <stdlib.h>
#include "operations.h"
#include "memory.h"
#include "memcycle.h"
#include "machine.h"
#include "um.h"
#include "assert.h"
void print_registers(Machine_T um);
void um_init_test();
void cond_move_test();
void
void blank_test()
{
    Machine_T um = Machine_new();
    printf("blank test begin\n");

    printf("blank test complete\n");
    Machine_free(&um);
}

int main() {
    um_init_test();
    cond_move_test();
    return 0;
}

void print_registers(Machine_T um)
{
    fprintf(stderr, "------------\n");
    for (int i = 0; i < 8; i++) {
        fprintf(stderr, "$R%i: %i\n", i, um->registers[i]);
    }
    fprintf(stderr, "------------\n");
}

void um_init_test()
{
    printf("initialize um test\n");
    UM_T um = UM_new(NULL);
    UM_free(&um);
    printf("initialize um complete\n");
}

void cond_move_test()
{
    Machine_T um = Machine_new();
    printf("conditional move test begin\n");

    um->registers[4] = 123456;
    um->registers[2] = 12;
    print_registers(um);
    cond_move(um, 0, 7, 4, 2);
    cond_move(um, 0, 3, 4, 0);
    fprintf(stderr, "moving r4 -> r7 if r2\n");
    fprintf(stderr, "moving r4 -> r3 if r0\n");
    cond_move(um, 0, 3, 2, 7);
    fprintf(stderr, "moving r2 -> r3 if r7\n");
    print_registers(um);
    assert(um->registers[7] == 123456);

    printf("conditional move test complete\n");
    Machine_free(&um);
}

void seg_test()
{
    Machine_T um = Machine_new();
    printf("seg load test begin\n");
    map(um, 0, 0, 0, 0, 0);
    map(um, 0, 0, 0, 0, 0);
    seg_store(um, 0, 0, 0, 0);
    seg_load(um, 0, 0, 0, 0);
    
    unmap(um, 0, 0, 0, 0, 0);

    printf("seg load test complete\n");
    Machine_free(&um);
}
