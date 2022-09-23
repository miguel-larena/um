/******************************************************************************
 *                              OPERATIONS.H
 *                         ----------------------
 *  Purpose: Declarations of the UM ISA operations,
 *           These will be put into an array when Machine_new is called from
 *           machine.h
 *
 *           The functions all take the same arguments
 *
 *  Notes:  The load_val function is slightly different, pass 25 bit value
 *         instead of rb
 *
 *  Dependencies: Memory.h memory unit
 *                machine.h virtual machine
 *
 *  By: Jacob Zimmerman and Miguel Larena
 *  Date: April 13 2022
 *****************************************************************************/

#ifndef OPERATIONS_INCLUDED
#define OPERATIONS_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "memory.h"
#include "machine.h"
#include <assert.h>

// extern Operation isa[14];
// /*
//  *    cond_move
//  *
//  *    Sets value of register a to b, if register c is 0.
//  */
// extern void cond_move(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *    seg_load
//  *
//  *    Loads word stored in register c of segment in register b to register a.
//  */
// extern void seg_load(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *    seg_store
//  *
//  *    Loads word stored in register a of segment in register b to register c.
//  */
// extern void seg_store(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *    add
//  *
//  *    Adds values of register a and b and stores the result in c.
//  */
// extern void add(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *    mult
//  *
//  *    Multiplies values of register a and b and stores the result in c.
//  */
// extern void mult(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *    divide
//  *
//  *    Divides values of register b and c and stores the result in c.
//  */
// extern void divide(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *    nand
//  *
//  *    Bitwise NAND's values of register b and c and stores the result in c.
//  */
// extern void nand(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *    halt
//  *
//  *    Stops all computation.
//  */
// extern void halt(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *      map
//  *
//  *      Creates a new segment with a number of words equal to the value
//  *      in register c and initializes each word in the new segment to 0.
//  *      A bit pattern that is not all 0s and that does not identify
//  *      any currently mapped segment is placed in register b.
//  *      A new segment maps to the value stored in register b.
//  */
// extern void map(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *      unmap
//  *
//  *      Unmaps segment stored in register c.
//  */
// extern void unmap(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *    out
//  *
//  *    Writes character stored in register c to i/o device.
//  *    Only values from 0 to 255 are allowed.
//  */
// extern void out(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *    in
//  *
//  *    Loads i/o device input to register c. Loads a full 32-bit word of all 1's
//  *    if EOF is reached.
//  */
// extern void in(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *    prog_load
//  *
//  *    Sets program counter to value in register c. Replaces the
//  *    current program segment with a duplicate of the segment stored in
//  *    segment id = $R[rb]
//  */
// extern void prog_load(Machine_T um, int ra, int rb, int rc);
//
// /*
//  *    load_val
//  *
//  *    Sets register a to value stored in argument value.
//  */
// static inline void load_val(Machine_T um, int ra, int value, int rc)
// {
//     (void)rc, (void)um, (void)ra, (void)value;
//     // um->registers[ra] = value;
// }

/* load segment, but disregard the jump */
static inline void prog_load_seg(Memory_T memory, u_int32_t segment_id)
{
    u_int32_t *segment = Memory_at(memory, segment_id);

    /* access secret length word */
    segment--;
    u_int32_t length = segment[0];

    /* copy whole segment including secret word */
    u_int32_t *prog_segment = malloc(sizeof(u_int32_t) * (length + 1));
    assert(prog_segment);
    memcpy(prog_segment, segment, sizeof(u_int32_t) * (length + 1));

    /* hide secret length word */
    segment++;
    prog_segment++;

    Memory_replace(memory, 0, prog_segment);
}


#endif
