/*
 * Copyright (c) 2008, 2009 Joseph Gaeddert
 * Copyright (c) 2008, 2009 Virginia Polytechnic Institute & State University
 *
 * This file is part of liquid-fpm.
 *
 * liquid-fpm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liquid-fpm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liquid-fpm.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIQUIDFPM_LIBVERSION_AUTOTEST_H
#define LIQUIDFPM_LIBVERSION_AUTOTEST_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <complex.h>

#include "liquidfpm.internal.h"
#include "autotest.h"

void libversion_autotest()
{
    // compare version numbers
    CONTEND_EXPRESSION(LIQUIDFPM_VERSION_NUMBER ==  liquidfpm_libversion_number());

    // compare version strings
    CONTEND_EXPRESSION(strcmp(LIQUIDFPM_VERSION,liquidfpm_libversion())==0);
}

#endif // LIQUIDFPM_LIBVERSION_AUTOTEST_H

