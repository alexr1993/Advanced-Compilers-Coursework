OBJS = analysis/lex.yy.o analysis/C.tab.o analysis/symbol_table.o analysis/nodes.o main.o
SRCS = analysis/lex.yy.c analysis/C.tab.c analysis/symbol_table.c analysis/nodes.c main.c
CC = gcc

CFLAGS = -Wall

all:	mycc

clean:
	rm ${OBJS}

mycc:	${OBJS}
	${CC} -g -o mycc ${OBJS}

# Generate lexical analyser with flex
lex.yy.c: analysis/C.flex
	flex analysis/C.flex

# Generate parser with bison
C.tab.c:	analysis/C.y
	bison -d -t -v analysis/C.y

.c.o:
	${CC} -g -c -o $@ $*.c

depend:
	${CC} -M $(SRCS) > .deps
	cat Makefile .deps > makefile

dist:	symbol_table.c nodes.c main.c Makefile C.flex C.y nodes.h token.h
	tar cvfz mycc.tgz symbol_table.c nodes.c main.c Makefile C.flex C.y \
		nodes.h token.h
