/******************************************************************************
 *                                 MEMCYCLE.H
 *                         ----------------------
 *  Purpose: Interface for memory recycling unit
 *           Tracks which segment ids have been unmapped and available
 *
 *           Current version is naive and uses only a single stack of ids,
 *           could be modified to include a smarter system that reduces
 *           malloc / free calls
 *
 *  Dependencies: None
 *
 *  By: Jacob Zimmerman and Miguel Larena
 *  Date: April 13 2022
 *****************************************************************************/

#ifndef MEMCYCLE_INCLUDED
#define MEMCYCLE_INCLUDED

#include <stdlib.h>

#define T Memcycle_T
typedef struct T *T;

/* New Memcycle unit */
extern T Memcycle_new();

/* Indicate that a segment id is available for reuse */
extern void Memcycle_open(T recycler, u_int32_t segment_id);

/* Return a segment_id from unit, returns 0 if empty */
extern u_int32_t Memcycle_get(T recycler);

/* Free Memcycle unit */
extern void Memcycle_free(T *open_spaces);

#undef T
#endif
