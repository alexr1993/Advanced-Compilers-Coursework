## TODO Clean up all the repitition in this file
EVALOBJS = evaluation/param.o \
           evaluation/function.o \
           evaluation/environment.o \
           evaluation/evaluate.o
EVALSRCS = evaluation/param.c \
           evaluation/function.c \
           evaluation/environment.c \
           evaluation/evaluate.c

ANALOBJS = analysis/lex.yy.o \
           analysis/C.tab.o \
           analysis/symbol_table.o \
           analysis/nodes.o \
  		   analysis/token.o
ANALSRCS = analysis/lex.yy.c \
           analysis/C.tab.c \
           analysis/symbol_table.c \
           analysis/nodes.c \
           analysis/token.c

COMMOBJS = util.o \
           common/operations.o
COMMSRCS = main.c \
           common/operations.co

TESTOBJS = tests/tests.o
TESTSRCS = tests/tests.c

OBJS = main.o
SRCS = main.c

CC = gcc

all:	mycc tests

clean:
	rm ${ANALOBJS} ${EVALOBJS} ${OBJS} ${COMMOBJS} ${TESTOBJS} \
       mycc analysis/C.tab.* analysis/lex.yy.c runtests

mycc:	 ${ANALSRCS} ${ANALOBJS} ${EVALOBJS} ${OBJS} ${COMMOBJS}
	${CC} -g -o mycc -I . ${ANALOBJS} ${EVALOBJS} ${OBJS} ${COMMOBJS}

# Generate lexical analyser with flex
analysis/lex.yy.c: analysis/C.flex
	cd analysis;flex C.flex; cd ..

# Generate parser with bison
analysis/C.tab.c:	analysis/C.y
	bison -d -t -v analysis/C.y -o analysis/C.tab.c

.c.o:
	${CC} -Wall -g -c -I . -o $@ $*.c

# FIXME
depend:
	${CC} -M $(ANALSRCS) $(EVALSRCS) $(SRCS)$(COMMOBJS) > .deps
	cat Makefile .deps > makefile

# FIXME
dist:	symbol_table.c nodes.c util.c main.c Makefile C.flex C.y nodes.h token.h
	tar cvfz mycc.tgz symbol_table.c nodes.c util.c main.c Makefile C.flex C.y \
		nodes.h token.h

tests: ${ANALOBJS} ${EVALOBJS} ${TESTOBJS} ${COMMOBJS}
	${CC} -g -o run_tests ${ANALOBJS} ${EVALOBJS} ${TESTOBJS} ${COMMOBJS} \
    `pkg-config --cflags --libs check`
