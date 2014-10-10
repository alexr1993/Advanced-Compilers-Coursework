SRCDIR = test_source

OBJS := $(wildcard ${SRCDIR}/*.o)
SRCS := $(wildcard ${SRCDIR}/*.c)

CC = gcc

all: ${OBJS}
	gcc -o run_tests tests.c `pkg-config --cflags --libs check`

.c.o:
	${CC} -g -c -o $@ $*.c

clean:
	rm ${OBJS}
