/******************************************************************************
 *                                  MEMORY.C
 *                         ----------------------
 *  Purpose: Implementation for a 32 bit addressed segmented virtual memory.
 *           Uses Memcycle_T to track reusable addresses.
 *           Segments is indexed four bytes at a time
 *
 *           The four bytes PRECEEDING the pointer to a segment contains a
 *           hidden word that tells you the length of the segment,
 *           (not including itself).
 *
 *           Only free segments using Segment_free().
 *
 *           Using uninitialized segment ids will likely result in
 *           Segmentation fault, be careful.
 *
 *           Garbage collection will deallocate all memory in the end.
 *
 *
 *  Data structures: Stores a map of segment_id to segment pointers in a
 *          Seq_T called segments.
 *
 *
 *  Dependencies: Memcycle.h memory recycler unit
 *
 *  By: Jacob Zimmerman and Miguel Larena
 *  Date: April 13 2022
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <seq.h>
#include <assert.h>
#include "memory.h"
#include "memcycle.h"


#define T Memory_T

/* Free a segment, including hidden byte */
static inline void Segment_free(u_int32_t *segment);

/* Safely zero segment at segment id and give it new size == length */
static inline void Memory_reuse_naive(T memory, u_int32_t segment_id,
                                      u_int32_t length);

static inline void Memory_reuse_experimental(T memory, u_int32_t segment_id,
                                             u_int32_t length);


static inline u_int32_t **Seggos_expand(T memory);

/* don't let this be 0 */
#define N_SEGMENTS_HINT 128


/* Memory_new
 *
 * Initialize a new memory unit
 *
 * Contains dictionary of segment_id -> addresses stored in Seq_T
 * Contains a recycler unit as a Memcycle_T
 *
 * Return Memory_T
 */
T Memory_new()
{
    T memory = malloc(sizeof(struct T));
    assert(memory);
    memory->recycler = Memcycle_new();
    memory->n_segments = 0;

    memory->seggos_cap = N_SEGMENTS_HINT;
    memory->seggos = malloc(sizeof(u_int32_t*) * memory->seggos_cap);
    assert(memory->seggos);

    return memory;
}

/* Memory_map
 *
 * Map a new segment of length <length>
 *
 * Will allocate a brand new segment id or re-use an unmapped one if possible,
 * hides the length of segment in hidden word.
 *
 * Return segment id of new segment
 */
u_int32_t Memory_map(T memory, u_int32_t length)
{
    /* try re-use a segment */
    u_int32_t segment_id = Memcycle_get(memory->recycler);
    if (segment_id != 0) {
        Memory_reuse_experimental(memory, segment_id, length);
        return segment_id;
    }

    /* need a new segment */
    u_int32_t *segment = malloc(sizeof(u_int32_t) * (length + 1));
    assert(segment);

    /* hidden length info */
    segment[0] = length;
    segment++;

    while (length) {
        length--;
        segment[length] = 0;
    }

    segment_id = memory->n_segments;
    memory->n_segments++;

    if (segment_id == memory->seggos_cap)
        memory->seggos = Seggos_expand(memory);

    memory->seggos[segment_id] = segment;

    return segment_id;
}

static inline u_int32_t **Seggos_expand(T memory)
{
    memory->seggos_cap <<= 1;
    return realloc(memory->seggos, sizeof(u_int32_t*) * memory->seggos_cap);
}

/* Memory_replace
 *
 * Safely change the segment stored at segment id
 *
 * Frees the old segment at segment id
 */
void Memory_replace(T memory, u_int32_t segment_id, u_int32_t *segment)
{
    Segment_free(memory->seggos[segment_id]);
    memory->seggos[segment_id] = segment;
}

/* Memory_reuse_naive
 *
 * A simple way of reallocating a segment length, relies heavily on malloc
 * and free.
 *
 * Segment at segment id now fits length number of words, all set to zero
 */
static inline void Memory_reuse_naive(T memory, u_int32_t segment_id,
                                      u_int32_t length)
{
    u_int32_t *segment = malloc(sizeof(u_int32_t) * (length + 1));
    assert(segment);

    /* hidden length word */
    segment[0] = length;
    segment++;

    /* zero out and re-insert into sequence */
    // memset(segment, 0, length * sizeof(u_int32_t));

    // Segment_free(memory->seggos[segment_id]);
    memory->seggos[segment_id] = segment;

    for (u_int32_t i = 0; i < length; i++)
        segment[i] = 0;
}

static inline void Memory_reuse_experimental(T memory, u_int32_t segment_id,
                                             u_int32_t length)
{

    u_int32_t *segment = malloc(sizeof(u_int32_t) * (length + 1));
    assert(segment);

    /* hidden length word */
    segment[0] = length;
    segment++;

    memory->seggos[segment_id] = segment;

    while (length) {
        length--;
        segment[length] = 0;
    }
}

/* Segment_free
 *
 * Actually free a segment, starting from hidden word
 */
static inline void Segment_free(u_int32_t *segment) {
    segment--;
    free(segment);
}

/* Memory_free
 *
 * Free memory_unit and all segments within it
 */
void Memory_free(T *memory)
{
    int n_segments = (*memory)->n_segments;
    u_int32_t **seggos = (*memory)->seggos;


    for (int i = 0; i < n_segments; i++) {
        if (seggos[i] != NULL)
            Segment_free(seggos[i]);
    }

    // fprintf(stderr, "NEW SEGMENT MALLOC CALLED:\n%i\n", (*memory)->counter);

    free(seggos);
    Memcycle_free(&(*memory)->recycler);
    free(*memory);
}
