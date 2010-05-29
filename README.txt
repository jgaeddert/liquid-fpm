======================================================================================

  Liquid/FPM : Software-Defined Radio Fixed-Point Math Library

======================================================================================

liquidfpm is a flexible fixed-point math library developed for efficient
computational software-defined radio.

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
 BUILD DESCRIPTION
======================================================================================

gentab (auto-generated tables)
Some of the algorithms in liquid-fpm (such as the CORDICs) use tables
which are automatically generated at build-time.
These are located in the gentab/ subdirectory.

For example,
    script:     gentab/gentab.sincos.cordic.c
    program:    gentab/gentab.sincos.cordic
    source:     gentab/sincos.cordic.$(qtype).c
    object:     gentab/sincos.cordic.$(qtype).o
where the makefile key $(qtype) is given by configure.ac (default is "q32")

genlib (auto-generated assembly objects)
Several of the low-level arithmetic functions (such as multiplication
of two fixed-point numbers) can be computed faster by taking advantage
of architecture-specific hardware instructions.
These functions are generated automatically given the precision...

======================================================================================
 CONFIGURING PRECISION
======================================================================================

The bit-precision of the fixed-point type is defined in two places:

configure.ac
    QTYPE                   : name of variables (e.g. "q32")
    QTYPE_INTBITS           : number of bits in integer component (e.g. "7")
    QTYPE_FRACBITS          : number of bits in fractional component (e.g. "25")
include/liquidfpm.h
    LIQUIDFPM_MANGLE        : name-mangling macro (e.g. LIQUIDFPM_CONCAT(q32,name))
    LIQUIDFPM_DEFINE_API    : interface macro

The default name for the data types are:
    q32_t   : fixed-point 32-bit data type
    cq32_t  : complex 2-valued (real,imag) 32-bit data type
    q32f_t  : floating-point 2-valued (base,frac) data type (internal use)

Limitations:
Due to some arithmetic constants required for computation in certain
algorithms, not all precisions are possible.  For example, the sinh|cosh
CORDIC requires initialization on kp_inv=1.207497... which needs at least 2
integer bits representation (not including the sign bit).  As a result, this
excludes data types such as Q1.31 format from running properly for these
functions. The library will configure and compile for such precisions, however
the unit tests might (and probaby will) fail.

======================================================================================
 SPECIFIC BUILD TARGETS
======================================================================================
Here is a list of the specific build targets

    all             :   everything: libliquidfpm shared object library
    check           :   generate autotest program and execute
    bench           :   generate benchmark program and execute

    distclean       :   clean everything, including configuration headers, and makefile
    clean           :   clean all ojbect files and shared library
    clean-genlib    :   clean only the auto-generated assembly sources and objects
    clean-gentab    :   clean only the auto-generated table sources and objects
    clean-bench     :   clean only the benchmark files
    clean-autotest  :   clean only the autotest files

    test            :   generate debugging test and execute
