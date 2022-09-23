#include <stdlib.h>
#include "memcycle.h"

#define T Memcycle_T

struct T {
    int count;
    struct elem {
        u_int32_t x;
        struct elem *link;
    } *head;
};

T Memcycle_new(void) {
    T recycler = malloc(sizeof(struct T));
    recycler->count = 0;
    recycler->head = NULL;
    return recycler;
}

void Memcycle_open(T recycler, u_int32_t x) {
    struct elem *t = malloc(sizeof(struct elem));
    t->x = x;
    t->link = recycler->head;
    recycler->head = t;
    recycler->count++;
}

u_int32_t Memcycle_get(T recycler) {
    if (recycler->head == NULL)
        return 0;

    struct elem *t = recycler->head;
    recycler->head = t->link;
    recycler->count--;
    u_int32_t x = t->x;
    free(t);

    return x;
}

void Memcycle_free(T *recycler) {
    struct elem *t, *u;
    for (t = (*recycler)->head; t; t = u) {
        u = t->link;
        free(t);
    }
    free(*recycler);
}
