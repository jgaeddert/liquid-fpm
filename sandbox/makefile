# Copyright (c) 2008, 2009, 2010 Joseph Gaeddert
# Copyright (c) 2008, 2009, 2010 Virginia Polytechnic
#                                Institute & State University
#
# This file is part of liquid.
#
# liquid is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# liquid is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with liquid.  If not, see <http://www.gnu.org/licenses/>.

# 
# Liquid fixed-point math sandbox makefile
#

CC	= gcc
CFLAGS	= -Wall -I.. -I../include
LDFLAGS	= -lm -lc ../libliquidfpm.a

src	=				\
	exp.shiftadd.test.c		\
	q32_exp.shiftadd.test.c		\
	log.shiftadd.test.c		\
	q32_log.shiftadd.test.c		\
	sincos.cordic.test.c		\
	q32_sincos.cordic.test.c	\
	sinhcosh.cordic.test.c		\
	q32_sinhcosh.cordic.test.c	\
	atan2.cordic.test.c		\
	q32_atan2.cordic.test.c		\
	loglogbesseli0.pwpoly.test.c	\
	q32_loglogbesseli0.pwpoly.test.c

obj	= $(patsubst %.c,%.o,$(src))
bin	= $(patsubst %.c,%,  $(src))

all : $(bin)

# object targets
%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

# program (binary) targets
$(bin) : % : %.o
	$(CC) -o $@ $< $(LDFLAGS)

# clean target
clean :
	rm -rf $(obj) $(bin)
