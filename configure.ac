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

#AC_PREREQ(2.65)
AC_INIT([liquid-fpm],[0.1.0],[support@liquidsdr.org])
AC_CONFIG_SRCDIR([src/libliquidfpm.c])

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
AC_PROG_CC
AC_PROG_SED
AC_PROG_GREP
AC_PROG_INSTALL
AC_PROG_RANLIB
#AC_PROG_MV
#AC_PROG_RM
#AC_PROG_AR
#AC_PROG_AS


# Check for necessary libraries
# AC_CHECK_LIB(c, malloc)
AC_CHECK_LIB(m, sinf)

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
AC_CHECK_HEADER(string.h,   [],[AC_MSG_ERROR([need string.h])])
AC_CHECK_HEADER(inttypes.h,   [],[AC_MSG_ERROR([need inttypes.h])])
AC_HEADER_STDBOOL

# Check for necessary library functions
AC_CHECK_FUNCS(floor,[],[AC_MSG_ERROR([need floor])])
AC_CHECK_FUNCS(sqrt,[],[AC_MSG_ERROR([need sqrt])])
AC_CHECK_FUNCS(strrchr,[],[AC_MSG_ERROR([need strrchr])])

# Check size of certain variables
AC_CHECK_SIZEOF(int)
# AC_CHECK_SIZEOF(unsigned int)

QTYPE="q32"
QTYPE_INTBITS="7"
QTYPE_FRACBITS="25"

# Check canonical system
AC_CANONICAL_TARGET
case $target_cpu in
i386|i486|i586|i686|x86)
    ARCH_OPTION=""
    GENLIBS="genlib/q32.x86.o"
    PORTLIBS=""
    AC_DEFINE(FPM_INTEL, 1, [Intel])
    ;;
x86_64)
    case $target_os in
    darwin*)
        GENLIBS="genlib/q32.intelmac.o"
        PORTLIBS=""
        ;;
    *)
        GENLIBS="genlib/q32.x86_64.o"
        PORTLIBS=""
        ;;
    esac

    AC_DEFINE(FPM_INTEL, 1, [Intel])
    ;;
ppc|powerpc*)
    ARCH_OPTION="-fno-common -faltivec"
    GENLIBS="genlib/q32.ppc.o"
    PORTLIBS=""
    AC_DEFINE(FPM_PPC, 1, [Power PC])
    ;;
arm)
    GENLIBS=""
    PORTLIBS="src/qtype_mul.port.o"
    AC_DEFINE(FPM_ARM, 1, [ARM])
    ;;
mips)
    GENLIBS=""
    PORTLIBS="src/qtype_mul.port.o"
    AC_DEFINE(FPM_MIPS, 1, [MIPS])
    ;;
sparc)
    GENLIBS=""
    PORTLIBS="src/qtype_mul.port.o"
    AC_DEFINE(FPM_SPARC, 1, [SPARC])
    ;;
64bit)
    GENLIBS=""
    PORTLIBS="src/qtype_mul.port.o"
    AC_DEFINE(FPM_64BIT, 1, [64-bit])
    ;;
*)
    GENLIBS=""
    PORTLIBS="src/qtype_mul.port.o"
    AC_DEFINE(FPM_DEFAULT, 1, [Default])
    ;;
esac

# override to portable types
PORTLIBS="src/qtype_mul.port.o"
GENLIBS=""

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

# GENLIBS defines the additional architecture-specific library objects to build.
# PORTLIBS defines the portable library objects to build
AC_SUBST(GENLIBS)
AC_SUBST(PORTLIBS)

# 
AC_SUBST(QTYPE)
AC_SUBST(QTYPE_INTBITS)
AC_SUBST(QTYPE_FRACBITS)

AC_SUBST(DEBUG_OPTION)

AC_CONFIG_FILES([makefile])
AC_OUTPUT

