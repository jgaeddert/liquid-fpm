======================================================================================

  Liquid/FPM : Software-Defined Radio Fixed-Point Math Library

======================================================================================

BUILD:
    $ ./reconf
    $ ./configure
    $ make

INSTALL:
    # make install

TEST:
    $ make test

BENCHMARK:
    $ make bench


======================================================================================
 BUILD
======================================================================================

gentab (auto-generated tables)
Some of the algorithms in liquid-fpm (such as the CORDICs) use tables
which are automatically generated at build-time.
These are located in the gentab/ subdirectory.

For example,
script:     gentab/gentab.sincos.cordic.c
program:    gentab/gentab.sincos.cordic
source:     gentab/sincos.cordic.q32.c
object:     gentab/sincos.cordic.q32.o

genlib (auto-generated assembly objects)
Several of the low-level arithmetic functions (such as multiplication
of two fixed-point numbers) can be computed faster by taking advantage
of architecture-specific hardware instructions.
These functions are generated automatically given the precision...

