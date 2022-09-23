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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "memory.h"
#include "machine.h"
#include "operations.h"

/*
 *    cond_move
 *
 *    Sets value of register a to b, if register c is 0.
 *
 *    Returns nothing.
 *
 */
void cond_move(Machine_T um, int ra, int rb, int rc)
{

    u_int32_t *registers = um->registers;
    // fprintf(stderr, "ra: %x rb: %x rc: %x\n", registers[ra], registers[rb], registers[rc]);
    if (registers[rc]) {
        registers[ra] = registers[rb];
    }
}

/*
 *    seg_load
 *
 *    Loads word stored in register c of segment in register b to register a.
 *
 *    Returns nothing.
 */
void seg_load(Machine_T um, int ra, int rb, int rc)
{
    u_int32_t *registers = um->registers;
    u_int32_t offset = registers[rc];
    registers[ra] = Memory_at(um->memory, registers[rb])[offset];
}

/*
 *
 *    seg_store
 *
 *    Loads word stored in register a of segment in register b to register c.
 *
 *    Returns nothing.
 */
void seg_store(Machine_T um, int ra, int rb, int rc)
{
    u_int32_t *registers = um->registers;
    u_int32_t offset = registers[rb];
    Memory_at(um->memory, registers[ra])[offset] = registers[rc];
}

/*
 *
 *    add
 *
 *    Adds values of register a and b and stores the result in c.
 *
 *    Returns nothing.
 */
void add(Machine_T um, int ra, int rb, int rc)
{
    u_int32_t *registers = um->registers;
    registers[ra] = registers[rb] + registers[rc];
}

/*
 *
 *    mult
 *
 *    Multiplies values of register a and b and stores the result in c.
 *
 *    Returns nothing.
 */
void mult(Machine_T um, int ra, int rb, int rc)
{
    u_int32_t *registers = um->registers;
    registers[ra] = registers[rb] * registers[rc];
}

/*
 *
 *    divide
 *
 *    Divides values of register b and c and stores the result in c.
 *
 *    Returns nothing.
 */
void divide(Machine_T um, int ra, int rb, int rc)
{
    u_int32_t *registers = um->registers;
    registers[ra] = registers[rb] / registers[rc];
}

/*
 *
 *    nand
 *
 *    Bitwise NAND's values of register b and c and stores the result in c.
 *
 *    Returns nothing.
 */
void nand(Machine_T um, int ra, int rb, int rc)
{
    u_int32_t *registers = um->registers;
    registers[ra] = ~(registers[rb] & registers[rc]);
}

/*
 *
 *    halt
 *
 *    Stops all computation.
 *
 *    Returns nothing.
 *
 */
void halt(Machine_T um, int ra, int rb, int rc)
{
    (void) um, (void) ra, (void) rb, (void) rc;
}

/*
 *
 *      map
 *
 *      Creates a new segment with a number of words equal to the value
 *      in register c and initializes each word in the new segment to 0.
 *      A bit pattern that is not all 0s and that does not identify
 *      any currently mapped segment is placed in register b.
 *      A new segment maps to the value stored in register b.
 *
 *      Returns nothing.
 *
 */
void map(Machine_T um, int ra, int rb, int rc)
{
    (void) ra;
    u_int32_t *registers = um->registers;
    u_int32_t segment_id = Memory_map(um->memory, registers[rc]);
    registers[rb] = segment_id;
}

/*
 *
 *      unmap
 *
 *      Unmaps segment stored in register c.
 *
 *      Returns nothing.
 *
 */
void unmap(Machine_T um, int ra, int rb, int rc)
{
    (void) ra, (void) rb;
    u_int32_t *registers = um->registers;
    Memory_unmap(um->memory, registers[rc]);
}

/*
 *
 *    out
 *
 *    Writes character stored in register c to i/o device.
 *    Only values from 0 to 255 are allowed.
 *
 *    Returns nothing.
 */
void out(Machine_T um, int ra, int rb, int rc)
{
    (void) ra, (void) rb;
    fprintf(um->output, "%c", (char)um->registers[rc]);
}

/*
 *
 *    in
 *
 *    Loads i/o device input to register c. Loads a full 32-bit word of all 1's
 *    if EOF is reached.
 *
 *    Returns nothing.
 */
void in(Machine_T um, int ra, int rb, int rc)
{
    (void) ra, (void) rb;
    int in = fgetc(um->input);
    if (in != EOF) {
        um->registers[rc] = in;
        return;
    }
    um->registers[rc] = 0xffffffff;
}

/*
 *
 *    prog_load
 *
 *    Sets program counter to value in register c. Replaces the
 *    current program segment with a duplicate of the segment stored in
 *    segment id = $R[rb]
 *
 *    Returns nothing.
 */
void prog_load(Machine_T um, int ra, int rb, int rc)
{
    (void) ra;
    u_int32_t *registers = um->registers;
    um->program_count = registers[rc];
    /* jump same segment */
    if (registers[rb] == 0) {
        return;
    }

    u_int32_t *segment = Memory_at(um->memory, registers[rb]);

    /* access secret length word */
    segment--;
    u_int32_t length = segment[0];

    /* copy whole segment including secret word */
    u_int32_t *prog_segment = malloc(sizeof(u_int32_t) * (length + 1));
    memcpy(prog_segment, segment, sizeof(u_int32_t) * (length + 1));

    /* hide secret length word */
    segment++;
    prog_segment++;

    Memory_replace(um->memory, 0, prog_segment);
}

/*
 *
 *    load_val
 *
 *    Sets register a to value stored in argument value.
 *
 *    Returns nothing.
 */
void load_val(Machine_T um, int ra, int value, int rc)
{
    (void) rc;
    um->registers[ra] = value;
}
