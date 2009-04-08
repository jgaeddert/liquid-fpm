======================================================================

  Liquid FPM : Fixed-Point Math Digital Signal Processing Library

======================================================================

This is the Liquid fixed-point math (lfpm) digital signal processing
library.  It is intended to provide an extensible library for FPM in
software-defined radios, particularly embedded systems where
floating-point math routines are either not available, or too slow for
practicality.


BUILD:
    $ ./reconf
    $ ./configure
    $ make

CONFIGURE OPTIONS:
    - fpm precision, data size (e.g. 16-bit, etc.)
    - target platform (e.g. ARM)

INSTALL:
    # make install

TEST: (requires python to generate header file)
    $ make check

PRECISION:
    $ make precision-test

BENCHMARK: (requires python to generate header file)
    $ make bench

Additional:
    benchmarking tool
    automatic test scripts
    full documentation (NOT doxygen)

Source code organization
liquidfpm/
    README.txt              <-- this file
    common.mk               <-- common makefile
    configure.in            <-- configuration shell script prototype
    makefile.in             <-- top-level makefile prototype
    reconf                  <-- bootstrapping shell script
    doc/
        makefile
        liquidfpm.tex
    include/
        liquidfpm.h         <-- external header, defines
                                all APIs for external use
        liquidfpm.internal.h<-- internal header
    src/
        module1/
            module.mk       <-- top-level included makefile for
                                building this specific module
            README
            src/
                object1.h   <-- internal header
                object1.1.c <-- internal source
                object1.2.c <-- internal source
                ...
            tests/
                autotest1.1.h
                autotest1.2.h
                ...
            bench/
                benchmark1.h
                benchmark2.h
                ...
        module2/
            ...
    examples/
        example.mk          <-- top-level included makefile
        example1.c
        example2.c

Documentation organization
    Introduction
    Usage
        #include <liquid/liquid.h>
    ...
    Module1
        Object1.1
            Interface
            Description of operation
            Other (Accuracy, etc.)
            Bugs
        Object1.2
            ...
    Module2
        ...

