# 
# Liquid fixed-point math sandbox makefile
#

CC	= gcc
CFLAGS	= -Wall -I.. -I../include
LDFLAGS	= -lm -lc ../libliquidfpm.a

src	=				\
	log.shiftadd.test.c		\
	q32_log.shiftadd.test.c		\
	sincos.cordic.test.c		\
	q32_sincos.cordic.test.c

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
