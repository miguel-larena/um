#include <stdlib.h>
#include "memcycle.h"
#include <stdio.h>
#define T Memcycle_T

struct T {
    int count;
    int half;
    struct elem {
        u_int32_t xf;
        u_int32_t xb;
        struct elem *link;
    } *head;
};

T Memcycle_new(void) {
    T recycler = malloc(sizeof(struct T));
    recycler->count = 0;
    recycler->half = 0;
    recycler->head = NULL;
    return recycler;
}

void Memcycle_open(T recycler, u_int32_t x) {
    recycler->half ^= 1;
    if (!recycler->half) { /* entry half full */
        recycler->head->xf = x;
    } else {
        struct elem *t = malloc(sizeof(struct elem));
        t->xb = x;
        t->link = recycler->head;
        recycler->head = t;
        recycler->count++;
    }
}

u_int32_t Memcycle_get(T recycler) {
    if (recycler->head == NULL)
        return 0;

    recycler->half ^= 1;
    if (recycler->half) { /* entry full */
        return recycler->head->xf;
    } else {
        struct elem *t = recycler->head;
        recycler->head = t->link;
        recycler->count--;
        u_int32_t x = t->xb;
        free(t);

        return x;
    }
}

void Memcycle_free(T *recycler) {
    struct elem *t, *u;
    for (t = (*recycler)->head; t; t = u) {
        u = t->link;
        free(t);
    }
    free(*recycler);
}
