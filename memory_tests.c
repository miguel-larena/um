#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "memtable.h"
#include "memcycle.h"
#include "assert.h"

typedef u_int32_t uint32;

static inline int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

void memory_test() {
    Memory_T memory = Memory_new();

    u_int32_t segment_id = Memory_map(memory, 1);
    printf("mapped seg id: %i", segment_id);
    u_int32_t *segment = Memory_at(memory, segment_id);
    segment[0] = 7;
    printf("segment[%i][0]:%i", segment_id, Memory_at(memory, segment_id)[0]);

    segment_id = Memory_map(memory, 3);
    printf("hmap seg id: %x\n", segment_id);
    segment = Memory_at(memory, segment_id);
    segment[0] = 8;
    printf("segment[%i][0]:%i", segment_id, Memory_at(memory, segment_id)[0]);
    Memory_unmap(memory, segment_id);

    segment_id = Memory_map(memory, 2);
    printf("fmap seg id: %x\n", segment_id);
    segment = Memory_at(memory, segment_id);
    segment[0] = 14;
    printf("segment[%i][0]:%i", segment_id, Memory_at(memory, segment_id)[0]);
    Memory_unmap(memory, segment_id);

    segment_id = Memory_map(memory, 4);
    printf("ramap seg id: %i\n", segment_id);
    segment = Memory_at(memory, segment_id);
    segment[0] = 21;
    printf("segment[%i][0]:%i", segment_id, Memory_at(memory, segment_id)[0]);
    Memory_unmap(memory, segment_id);

    fprintf(stderr, "made it to last unmap call!\n");

    Memory_unmap(memory, 0);
    Memory_free(&memory);
    return;
}

void malloc_free_test() {
    for (int i = 0; i < 1000000; i++) {
        u_int32_t *seg = malloc(sizeof(u_int32_t) * 10);
        free(seg);
    }
}

void malloc_free_test_heavy() {
    for (u_int32_t i = 1; i < 10000; i++) {
        int size1 = max(1, i ^ 0xa2132);
        int size2 = max(1, i ^ 0x741e2);

        u_int32_t *seg1 = malloc(sizeof(u_int32_t) * size1);
        u_int32_t *seg2 = malloc(sizeof(u_int32_t) * size2);

        for (int j = 0; j < size1; j++) {
            seg1[j] = i;
        }
        for (int j = 0; j < size2; j++) {
            seg2[j] = i;
        }

        int cond1 = seg1[0] == i;
        int cond2 = seg2[0] == i;
        if (!(cond1 && cond2)) {
            fprintf(stderr,
                    "FAILURE!\n");
        }

        free(seg1);
        free(seg2);
    }
}

void memory_repeats_test() {
    Memory_T memory = Memory_new();
    Memory_map(memory, 50);
    printf("Starting 10000 map->unmap calls\n");
    for (int i = 0; i < 1000000; i++) {
        u_int32_t id = Memory_map(memory, 10);
        Memory_unmap(memory, id);
    }
    printf("map->unmap complete\n");

    printf("calling memory_at 10000 times\n");
    for (int i = 0; i < 1000000; i++) {
        Memory_at(memory, 0)[i % 50] += 5;
    }
    printf("memory_at completed\n");

    printf("Starting 10000 map calls\n");
    u_int32_t *ids = malloc(sizeof(u_int32_t) * 1000000);
    for (int i = 0; i < 1000000; i++) {
        ids[i] = Memory_map(memory, 10);
    }
    printf("Complete 10000 map calls\n");
    printf("Starting 10000 unmap calls\n");
    for (int i = 0; i < 1000000; i++) {
        Memory_unmap(memory, ids[i]);
    }
    printf("10000 unmap complete\n");

    Memory_free(&memory);
}

void memory_test_heavy() {
    Memory_T memory = Memory_new();

    Memory_map(memory, 10);

    for (u_int32_t i = 1; i < 10000; i++) {

        int size1 = max(1, i ^ 0xa32);
        int size2 = max(1, i ^ 0x1e2);

        u_int32_t id1 = Memory_map(memory, size1);
        u_int32_t id2 = Memory_map(memory, size2);

        for (int j = 0; j < size1; j++) {
            Memory_at(memory, id1)[0] = i;
        }
        for (int j = 0; j < size2; j++) {
            Memory_at(memory, id2)[0] = i;
        }

        int cond1 = Memory_at(memory, id1)[0] == i;
        int cond2 = Memory_at(memory, id2)[0] == i;
        if (!(cond1 && cond2)) {
            fprintf(stderr,
                    "FAILURE!\nm[%i]-> %i != %i or \nm[%i] -> %i != %i\n",
                    id1, Memory_at(memory, id1)[0], i,
                    id2, Memory_at(memory, id2)[0], i);
        }

        if ((i ^ 0xa32) == 0 || (i ^ 0x1e2) == 0) {
            continue;
        }

        Memory_unmap(memory, id1);
        Memory_unmap(memory, id2);
    }

    for (int i = 0; i < 10000; i++) {
        if ((i ^ 0xa32) == 0 || (i ^ 0x1e2) == 0) {
            assert(Memory_at(memory, i)[0] == (u_int32_t)i);
        }
    }

    Memory_free(&memory);
}

int main()
{
    printf("------\nMemory test\n");
    memory_test();

    // printf("------\nMemory test heavy\n");
    // memory_test_heavy();
    //
    // printf("------\n Memory unmap map test heavy\n");
    // memory_repeats_test();
    //
    // printf("------\n malloc free test heavy\n");
    // malloc_free_test();

    return 0;
}
