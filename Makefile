## TODO Clean up all the repitition in this file
ANALOBJS = analysis/lex.yy.o \
           analysis/C.tab.o \
           analysis/symbol_table.o \
           analysis/nodes.o \
           analysis/environment.o \
  		   analysis/token.o \

ANALSRCS = analysis/lex.yy.c \
           analysis/C.tab.c \
           analysis/symbol_table.c \
           analysis/nodes.c \
           analysis/environment.c \
           analysis/token.c \

SYNTHOBJS = synthesis/interpret.o \
            synthesis/tac.o \
            synthesis/mips.o \
			synthesis/evaluate.o \
			synthesis/operations.o \

SYNTHSRCS = synthesis/interpret.c \
            synthesis/tac.c \
            synthesis/mips.c \
  			synthesis/evaluate.c \
 			synthesis/operations.c \


COMMOBJS = common/util.o \

COMMSRCS = common/util.c \

TESTOBJS = t/tests.o \
           t/frontend.o \
           t/interpret.o \
		   t/tac.o \
           t/mips.o \

TESTSRCS = t/tests.c \
           t/frontend.c \
           t/interpret.c \
		   t/tac.c \
           t/mips.c \

OBJS = main.o
SRCS = main.c

CC = gcc

all:	mycc tests
	./t/build_tests.sh

clean:
	rm ${ANALOBJS} ${SYNTHOBJS} ${OBJS} ${COMMOBJS} ${TESTOBJS} \
       mycc analysis/C.tab.* analysis/lex.yy.c run_tests

mycc:	 ${ANALSRCS} ${ANALOBJS} ${SYNTHOBJS} ${OBJS} ${COMMOBJS}
	${CC} -g -o mycc -I . ${ANALOBJS} ${SYNTHOBJS} ${OBJS} ${COMMOBJS}

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
	${CC} -M $(ANALSRCS) $(SYNTHSRCS) $(SRCS)$(COMMOBJS) > .deps
	cat Makefile .deps > makefile

# FIXME
dist:	symbol_table.c nodes.c util.c main.c Makefile C.flex C.y nodes.h token.h
	tar cvfz mycc.tgz symbol_table.c nodes.c util.c main.c Makefile C.flex C.y \
		nodes.h token.h

tests: ${ANALSRCS} ${ANALOBJS} ${SYNTHOBJS} ${TESTOBJS} ${COMMOBJS}
	${CC} -g -o run_tests -lcheck ${ANALOBJS} ${SYNTHOBJS} ${TESTOBJS} ${COMMOBJS}
