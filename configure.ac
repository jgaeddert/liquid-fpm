##
## Copyright (c) 2008, 2009, 2010 Joseph Gaeddert
## Copyright (c) 2008, 2009, 2010 Virginia Polytechnic
##                                Institute & State University
##
## This file is part of liquid.
##
## liquid is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## liquid is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with liquid.  If not, see <http://www.gnu.org/licenses/>.
##

# 
# Liquid fixed-point math library configure
# Process with autoconf to generate configure script
#

AC_INIT([liquid-fpm],[0.1.0],[liquid@vt.edu])
AC_CONFIG_SRCDIR([libliquidfpm.c])

# Autoheader
AC_CONFIG_HEADER(config.h)
AH_TOP([
#ifndef __LIQUIDFPM_CONFIG_H__
#define __LIQUIDFPM_CONFIG_H__
])
AH_BOTTOM([
#endif // __LIQUIDFPM_CONFIG_H__
])

# Configure options

AC_ARG_ENABLE(debug,
    AS_HELP_STRING([--enable-debug],[debug]),
    [DEBUG_OPTION="-DDEBUG"],
    [DEBUG_OPTION=""]
)

# Check for necessary programs
AC_PROG_INSTALL
AC_PROG_RANLIB

# Check for necessary libraries
# AC_CHECK_LIB(c, malloc)
# AC_CHECK_LIB(m, sinf)

# Check for necessary C specifics
AC_C_CONST
AC_C_INLINE
AC_TYPE_UINT8_T
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T

# Check for necessary header files
AC_CHECK_HEADER(stdio.h,    [],[AC_MSG_ERROR([need stdio.h])])
AC_CHECK_HEADER(stdlib.h,   [],[AC_MSG_ERROR([need stdlib.h])])
AC_HEADER_STDBOOL

# Check for necessary library functions
AC_CHECK_FUNC(memmove,[],[AC_MSG_ERROR([need floor])])

# Check size of certain variables
AC_CHECK_SIZEOF(int)
# AC_CHECK_SIZEOF(unsigned int)

QTYPE="q32"
QTYPE_INTBITS="7"
QTYPE_FRACBITS="25"

# Check canonical system
AC_CANONICAL_TARGET
case $target_cpu in
i386|i486|i586|i686)
    # ARCH_OPTION="-march=$target_cpu"
    ARCH_OPTION=""
    FPM_OPTION="-DFPM_INTEL"

    case $target_os in
    darwin*)
        MLIBS="genlib/q32.intelmac.o"
        ;;
    *)
        # MLIBS="q32_mul.x86.o"
        MLIBS="genlib/q32.x86.o"
        ;;
    esac

    AC_DEFINE(FPM_INTEL, 1, [Intel])
    ;;
ppc|powerpc*)
    ARCH_OPTION="-fno-common -faltivec"
    FPM_OPTION="-DFPM_PPC"
    MLIBS="genlib/q32.ppc.o"
    AC_DEFINE(FPM_PPC, 1, [Power PC])
    ;;
arm)
    FPM_OPTION="-DFPM_ARM"
    MLIBS="q32_mul.port.o"
    AC_DEFINE(FPM_ARM, 1, [ARM])
    ;;
mips)
    FPM_OPTION="-DFPM_MIPS"
    MLIBS="q32_mul.port.o"
    AC_DEFINE(FPM_MIPS, 1, [MIPS])
    ;;
sparc)
    FPM_OPTION="-DFPM_SPARC"
    MLIBS="q32_mul.port.o"
    AC_DEFINE(FPM_SPARC, 1, [SPARC])
    ;;
64bit)
    FPM_OPTION="-DFPM_64BIT"
    MLIBS="q32_mul.port.o"
    AC_DEFINE(FPM_64BIT, 1, [64-bit])
    ;;
*)
    FPM_OPTION="-DFPM_DEFAULT"
    MLIBS="q32_mul.port.o"
    AC_DEFINE(FPM_DEFAULT, 1, [Default])
    ;;
esac

case $target_os in
darwin*)
    SH_LIB=libliquidfpm.dylib
    REBIND=""
    ;;
*)
    SH_LIB=libliquidfpm.so
    REBIND=ldconfig
    ;;
esac
AC_SUBST(SH_LIB)
AC_SUBST(REBIND)
AC_SUBST(ARCH_OPTION)

# MLIBS defined the additional architecture-specific library objects to
# build.
AC_SUBST(MLIBS)

# 
AC_SUBST(QTYPE)
AC_SUBST(QTYPE_INTBITS)
AC_SUBST(QTYPE_FRACBITS)

AC_SUBST(DEBUG_OPTION)
AC_SUBST(FPM_OPTION)

AC_CONFIG_FILES([makefile])
AC_OUTPUT
