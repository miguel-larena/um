/******************************************************************************
 *                               MACHINE.H
 *                         ----------------------
 *  Purpose: The contents of the Machine_T struct is the entire
 *            virtual machine. The following functions help developers,
 *            NOT CLIENTS, to access and manipulate under the hood of the VM.
 *            Again, this is not a client facing type.
 *
 *  Notes:   The Machine_T struct is declared in the header file so it
 *             can be manipulated anywhere that machine.h is included
 *
 *  Dependencies: Memory.h virtual memory module
 *
 *  By: Jacob Zimmerman and Miguel Larena
 *  Date: April 13 2022
 *****************************************************************************/

#ifndef INSTRUCTION_INCLUDED
#define INSTRUCTION_INCLUDED

#include "memory.h"

// #define T Machine_T

// typedef struct T *T;
typedef u_int32_t Instruction;
/* all operation function pointers use this type */
// typedef void (*Operation)(T, int, int, int);

/* execute argument Instruction, returns 1 if HALT, 0 otherwise */
extern void Machine_run(FILE *program);


#undef T
#endif
