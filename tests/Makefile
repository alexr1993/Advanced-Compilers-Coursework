OBJS = test_source/basic_arithmetic.o
SRCS = test_source/basic_arithmetic.c
CC = gcc

all: ${OBJS}
	gcc -o interpret_tests interpret.c `pkg-config --cflags --libs check`

.c.o:
	${CC} -g -c -o $@ $*.c
	rm ${OBJS}
