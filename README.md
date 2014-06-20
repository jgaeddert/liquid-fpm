
liquid-fpm : Software-Defined Radio Fixed-Point Math Library
============================================================

*NOTE:* This repository has been incorporated as a branch in the main
liquid-dsp repository here:
[https://github.com/jgaeddert/liquid-dsp](https://github.com/jgaeddert/liquid-dsp)


liquidfpm is a flexible fixed-point math library developed for efficient
computational software-defined radio.

Build:

    $ ./bootstrap.sh
    $ ./configure
    $ make

Install:

    # make install

Run tests:

    $ make test

Run benchmarks:

    $ make bench


## ANGULAR REPRESENTATION ##

## INTERFACES ##

Reference

    N : total number of bits in the data type (e.g. 32)
    I : total number of integer bits, including sign
    F : total number of fractional bits

Regular constants

    name                value                   description
    qtype_min                                   minimum representable value
    qtype_max                                   maximum representable value
    qtype_one           1                       one
    qtype_pi                                    pi (angular)
    qtype_2pi                                   2*pi (angular)
    qtype_pi_by_2                               pi/2 (angular)
    qtype_angle_scalarf 3.14159 / 2^(I-2)       angular <-> scalar relationship

Mathematical constants (genlib/genlib.constants.c)

    name                value                   description
    qtype_E             2.71828182845905        e
    qtype_LOG2E         1.44269504088896        log2(e)
    qtype_LOG10E        0.434294481903252       log10(e)
    qtype_LN2           0.693147180559945       log(2)
    qtype_LN10          2.30258509299405        log(10)
    qtype_PI            3.14159265358979        pi (actual)
    qtype_PI_2          1.57079632679490        pi/2
    qtype_PI_4          0.785398163397448       pi/4
    qtype_1_PI          0.318309886183791       1/pi
    qtype_2_PI          0.636619772367581       2/pi
    qtype_2_SQRTPI      1.12837916709551        2/sqrt(pi)
    qtype_SQRT2         1.41421356237310        sqrt(2)
    qtype_SQRT1_2       0.707106781186547       1/sqrt(2)

Fixed-point/Floating-point conversion

    qtype_fixed_to_float            convert fixed-point value to floating-point value
    qtype_float_to_fixed            convert floating-point value to fixed-point value
    qtype_angle_fixed_to_float      convert fixed-point angle to floating-point angle
    qtype_angle_float_to_fixed      convert floating-point value to fixed-point angle

Basic arithmetic functions

    qtype_sign(x)       sign of x = x / |x|
    qtype_abs(x)        |x|
    qtype_add(x,y)      x+y
    qtype_sub(x,y)      x-y
    qtype_mul(x,y)      x*y
    qtype_div(x,y)      x/y
    qtype_inv(x,n)      1/x with n iterations
    qtype_ratio(a,b,n)  integer ratio a/b with n iterations

Pre-C99 functions

    qtype_acos
    qtype_atan
    qtype_atan2
    qtype_ceil
    qtype_cos
    qtype_cosh
    qtype_exp
    qtype_fabs
    qtype_floor
    qtype_fmod
    qtype_frexp
    qtype_ldexp
    qtype_log
    qtype_log10
    qtype_modf
    qtype_pow
    qtype_sin
    qtype_sinh
    qtype_sqrt
    qtype_tan
    qtype_tanh

C99 functions

    qtype_acosh
    qtype_asinh
    qtype_atanh
    qtype_cbrt
    qtype_copysign
    qtype_erf
    qtype_erfc
    qtype_exp2
    qtype_expm1
    qtype_fdim
    qtype_fma
    qtype_fmax
    qtype_fmin
    qtype_hypot
    qtype_ilogb
    qtype_lgamma
    qtype_llrint
    qtype_lrint
    qtype_llround
    qtype_lround
    qtype_log1p
    qtype_log2
    qtype_logb
    qtype_nan
    qtype_nearbyint
    qtype_nextafter
    qtype_nexttoward
    qtype_remainder
    qtype_remquo
    qtype_rint
    qtype_round
    qtype_scalbln
    qtype_scalbn
    qtype_tgamma
    qtype_trunc

Additional functions

    qtype_lngamma
    qtype_besseli0
    qtype_besselj0
    qtype_sinc
    qtype_kaiser

Complex arithmetic

    cqtype_add(x,y)         x+y
    cqtype_sub(x,y)         x-y
    cqtype_mul(x,y)         x*y
    cqtype_mul_scalar(x,c)  x*c
    cqtype_div(x,y)         x/y
    cqtype_div_scalar(x,c)  x/c
    cqtype_inv(x)           1/x

Complex trig

    cqtype_cabs
    cqtype_cacos
    cqtype_cacosh
    cqtype_carg
    cqtype_casin
    cqtype_casinh
    cqtype_catan
    cqtype_catanh
    cqtype_ccos
    cqtype_ccosh
    cqtype_cexp
    cqtype_cimag
    cqtype_clog
    cqtype_conj
    cqtype_cpow
    cqtype_cproj
    cqtype_creal
    cqtype_csin
    cqtype_csinh
    cqtype_csqrt
    cqtype_ctan
    cqtype_ctanh

 * Function declaration exists but no function definition (not yet implemented)

## Build Description ##

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

## Configuring Precision ##

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

## Specific Build Targets ##

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
