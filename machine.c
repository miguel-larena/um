/******************************************************************************
 *                               MACHINE.C
 *                         ----------------------
 *  Purpose: This is the implementation for the virtual machine
 *              functions. This also defines the macros for unpacking
 *              instruction details from binary words.
 *
 *  Notes:   The Machine_T stores an array of function pointers as it's ISA
 *
 *  Dependencies: Memory.h virtual memory module
 *                Operations.h static inline functions to operate on the VM
 *
 *  By: Jacob Zimmerman and Miguel Larena
 *  Date: April 13 2022
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "machine.h"
#include "operations.h"
#include "memory.h"

#define T Machine_T

#define N_OPERATIONS  14
#define N_REGISTERS    8

#define HALT_CODE      7
#define LOAD_VAL_CODE 13

/* Bit unpacking macros MUST be used with an u_int32_t type argument */
#define RA_MASK                 0x000001c0
#define RB_MASK                 0x00000038
#define RC_MASK                 0x00000007
#define LD_RA_MASK              0x0e000000
#define LD_VAL_MASK             0x01ffffff
#define I_READ_RA(inst)         (((inst) & RA_MASK) >> 6)
#define I_READ_RB(inst)         (((inst) & RB_MASK) >> 3)
#define I_READ_RC(inst)         ((inst) & RC_MASK)
#define INST_READ_OP(inst)      ((inst) >> 28)
#define INST_LD_READ_RA(inst)   ((inst) << 4 >> 29)
#define INST_LD_READ_VAL(inst)  ((inst) << 7 >> 7)

#define I_READ_ABC(inst, a, b, c) { \
        c = inst & RC_MASK;         \
        inst >>= 3;                 \
        b = inst & RC_MASK;         \
        inst >>= 3;                 \
        a = inst & RC_MASK;         \
}

#define I_READ_BC(inst, b, c) {     \
        c = inst & RC_MASK;         \
        inst >>= 3;                 \
        b = inst & RC_MASK;         \
}

void Memory_load_program(Memory_T memory, FILE *fp) {
    /* find program size */
    fseek(fp, 0, SEEK_END);
    size_t n_inst = ftell(fp) / 4;
    rewind(fp);

    /* allocate space in program segment */
    Memory_map(memory, n_inst);
    u_int32_t *segment = Memory_at(memory, 0);

    /* load program */
    fread(segment, sizeof(u_int32_t), n_inst, fp);
    for (size_t i = 0; i < n_inst; i++) {
        segment[i] = __builtin_bswap32(segment[i]);
    }
}

/*
 *      Machine_run
 *
 *   While not halt, execute next instruction.
 */
void Machine_run(FILE *fp)
{
    // int histogram[14] = {0};
    int opcode, a, b, c;
    u_int32_t r[8] = {0};
    Memory_T memory = Memory_new();
    Memory_load_program(memory, fp);

    u_int32_t *program = Memory_at(memory, 0);
    FILE *output = stdout;
    FILE *input = stdin;

    do { /* while opcode != HALT */

        Instruction inst = *program;
        program++;

        opcode = INST_READ_OP(inst);

        if (opcode == 13) {
            a = INST_LD_READ_RA(inst), b = INST_LD_READ_VAL(inst);
            r[a] = b;
        } else {
            switch (opcode) {
            case 0: /* conditional move */
                a = I_READ_RA(inst), b = I_READ_RB(inst), c = I_READ_RC(inst);
                // I_READ_ABC(inst, a, b, c)
                if (r[c])
                    r[a] = r[b];
                break;

            case 1: /* seg load */
                // a = I_READ_RA(inst), b = I_READ_RB(inst), c = I_READ_RC(inst);
                I_READ_ABC(inst, a, b, c)
                r[a] = Memory_at(memory, r[b])[r[c]];
                break;

            case 2: /* seg store */
                // a = I_READ_RA(inst), b = I_READ_RB(inst), c = I_READ_RC(inst);
                I_READ_ABC(inst, a, b, c)
                Memory_at(memory, r[a])[r[b]] = r[c];
                break;

            case 3: /* add */
                a = I_READ_RA(inst), b = I_READ_RB(inst), c = I_READ_RC(inst);
                // I_READ_ABC(inst, a, b, c)
                r[a] = r[b] + r[c];
                break;

            case 4: /* multiply */
                a = I_READ_RA(inst), b = I_READ_RB(inst), c = I_READ_RC(inst);
                // I_READ_ABC(inst, a, b, c)
                r[a] = r[b] * r[c];
                break;

            case 5: /* divide */
                a = I_READ_RA(inst), b = I_READ_RB(inst), c = I_READ_RC(inst);
                // I_READ_ABC(inst, a, b, c)
                r[a] = r[b] / r[c];
                break;

            case 6: /* nand */
                a = I_READ_RA(inst), b = I_READ_RB(inst), c = I_READ_RC(inst);
                // I_READ_ABC(inst, a, b, c)
                r[a] = ~(r[b] & r[c]);
                break;

            case 7:
                (void)output;
                break;

            case 8: /* map */
                // b = I_READ_RB(inst), c = I_READ_RC(inst);
                I_READ_BC(inst, b, c)
                r[b] = Memory_map(memory, r[c]);
                break;

            case 9: /* unmap */
                c = I_READ_RC(inst);
                Memory_unmap(memory, r[c]);
                break;

            case 10: /* output */
                c = I_READ_RC(inst);
                fprintf(output, "%c", r[c]);
                break;

            case 11: /* input */
                c = I_READ_RC(inst);
                int inchar = fgetc(input);
                if (inchar != EOF)
                    r[c] = inchar;
                else
                    r[c] = 0xffffffff;
                break;

            case 12: /* prog load */
                b = I_READ_RB(inst), c = I_READ_RC(inst);
                // I_READ_BC(inst, b, c)
                if (r[b])
                    prog_load_seg(memory, r[b]);
                program = Memory_at(memory, 0) + r[c];
                break;

            // case 13: /* load val */
            //     a = INST_LD_READ_RA(inst), b = INST_LD_READ_VAL(inst);
            //     r[a] = b;
            //     break;
            }
        }

    } while (opcode != HALT_CODE);

    Memory_free(&memory);
}



#undef T
