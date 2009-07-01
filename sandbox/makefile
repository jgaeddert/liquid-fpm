

CC	= gcc
CFLAGS	= -Wall -I.. -I../include ../libliquidfpm.a -lm -lc

src	=			\
	log.shiftadd.test.c	\
	q32_log.shiftadd.test.c	\
	sincos.cordic.test.c

obj	= $(patsubst %.c,%.o,$(src))
bin	= $(patsubst %.c,%,  $(src))

all : $(bin)

clean :
	rm -rf $(obj) $(bin)
