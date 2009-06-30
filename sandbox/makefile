

CC	= gcc
CFLAGS	= -I.. -I../include ../libliquidfpm.a

src	=			\
	log.shiftadd.test.c	\
	q32_log.shiftadd.test.c

obj	= $(patsubst %.c,%.o,$(src))
bin	= $(patsubst %.c,%,  $(src))

all : $(bin)

clean :
	rm -rf $(obj) $(bin)
