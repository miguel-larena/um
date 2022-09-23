/******************************************************************************
 *                                 MEMORY.H
 *                         ----------------------
 *  Purpose: Interface for a 32 bit addressed virtual memory. Stores segmented
 *          memory of 4 byte words. Interface supports mapping, unmapping,
 *          retrieving and replacing segments.
 *
 *          Segments are initialized with all zeros.
 *
 *          Using uninitialized segment ids will likely result in
 *          Segmentation fault, be careful.
 *
 *  Notes:  Garbage collection will deallocate all memory in the end.
 *          Client MUST!!! not attempt to free any segments on their own.
 *          (It will cause memory leak...)
 *
 *
 *  Dependencies: Memcycle.h memory recycler unit
 *
 *  By: Jacob Zimmerman and Miguel Larena
 *  Date: April 13 2022
 *****************************************************************************/

#ifndef MEMORY_INCLUDED
#define MEMORY_INCLUDED

#include <stdlib.h>
#include <stdint.h>
#include "memcycle.h"

#define T Memory_T
typedef struct T *T;

struct T {
    u_int32_t **seggos;
    Memcycle_T recycler;
    u_int32_t n_segments;
    u_int32_t seggos_cap;
    u_int32_t why_does_this_make_the_program_faster_it_literally_does_nothing;
    // u_int32_t another;
};


/*  New virtual memory unit, returns pointer to Memory_T struct */
extern T Memory_new();

/*  Map new segment with <length> number of words, return segment id */
extern u_int32_t Memory_map(T memory, u_int32_t length);

/*  Unmap segment at virtual address <segment_id> */
// extern void Memory_unmap(T memory, u_int32_t segment_id);

static inline void Memory_unmap(T memory, u_int32_t segment_id)
{
    u_int32_t *segment = memory->seggos[segment_id];

    segment--;
    free(segment);

    memory->seggos[segment_id] = NULL;

    Memcycle_open(memory->recycler, segment_id);
}

/*  Return pointer to physical memory address of segment at <segment_id>
 *
 *  Passing an unmapped segment id is undefined behaviour
 */
// extern u_int32_t *Memory_at(T memory, u_int32_t segment_id);

static inline u_int32_t *Memory_at(T memory, u_int32_t segment_id)
{
    return memory->seggos[segment_id];
}

/*  Free the current segment at segment_id,
 *  replace it with segment <segment>.
 *
 *  Passing an umapped segment id is undefined behaviour
 */
extern void Memory_replace(T memory, u_int32_t segment_id, u_int32_t *segment);

/* Free Memory_T and every segment within Memory Unit, mapped or not */
extern void Memory_free(T *memory);

#undef T
#endif
