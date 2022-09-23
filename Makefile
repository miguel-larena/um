# # Makefile for um (Comp 40 Assignment 6)
# #
# # Authors: Jacob Zimmerman and Miguel Larena
# # Date: April 14, 2022
# #
# # Compile with make um
# #
# #

############## Variables ###############

CC = gcc # The compiler being used

# Updating include path to use Comp 40 .h files and CII interfaces
IFLAGS = -I/comp/40/build/include -I/usr/sup/cii40/include/cii
OFLAGS := -flto -Ofast
SLOFLAGS := -O0

CFLAGS := -g -std=gnu99 $(IFLAGS)
CFLAGS += -Wall -Wextra -Werror -Wfatal-errors -pedantic
CFLAGS += $(OFLAGS)


# Linking flags
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64

# Libraries needed for linking
# All programs cii40 (Hanson binaries) and *may* need -lm (math)
LDLIBS = -lcii40-O2 -lm -lrt

INCLUDES = $(shell echo *.h)

############### Rules ###############

all: um

## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

memcycle_fast.o: memcycle_complex.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

# operations.h: opfast.h

## Linking step (.o -> executable program)

bench: benchmark.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

branch: branchmark.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

um: main.o um.o memory.o memcycle.o machine.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

umfast: main.o memcycle_fast.o memory.o machine.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

submit:
	submit40-um memory.c memory.h machine.c machine.h um.c um.h operations.c \
	operations.h main.c memcycle.c memcycle.h README UMTESTS \
	chars.um char.0 chars.1 \
	halt.um halt.1 \
	halt-verbose.um halt-verbose.1 \
	print-six.um print-six.1 \
	print-string.um print-string.1 \
	subtract.um subtract.1 \
	math.um math.1 \
	everything-bagel.um everything-bagel.0 everything-bagel.1 \
	io.um io.1 \
	map-unmap-edge.um map-unmap-edge.1 \
	map-huge.um \
	divisor-big.um divisor-big.1 \
	divisor-0.um divisor-0.1 \
	cond-test.um cond-test.1 \
	load-prog.um load-prog.1 \
	jump-edge.um jump-edge.1

clean:
	rm -f memory *.o
