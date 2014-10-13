## TODO Clean up all the repitition in this file
OBJS = evaluate.o analysis/lex.yy.o analysis/C.tab.o analysis/symbol_table.o analysis/nodes.o util.o main.o
SRCS = evaluate.c analysis/lex.yy.c analysis/C.tab.c analysis/symbol_table.c analysis/nodes.c util.c main.c
CC = gcc

CFLAGS = -Wall

all:	mycc

clean:
	rm ${OBJS}

mycc:	${OBJS}
	${CC} -g -o mycc ${OBJS}
	rm ${OBJS}

# Generate lexical analyser with flex
lex.yy.c: analysis/C.flex
	flex analysis/C.flex

# Generate parser with bison
C.tab.c:	analysis/C.y
	bison -d -t -v analysis/C.y

.c.o:
	${CC} -g -c -o $@ $*.c

# FIXME
depend:
	${CC} -M $(SRCS) > .deps
	cat Makefile .deps > makefile

# FIXME
dist:	symbol_table.c nodes.c util.c main.c Makefile C.flex C.y nodes.h token.h
	tar cvfz mycc.tgz symbol_table.c nodes.c util.c main.c Makefile C.flex C.y \
		nodes.h token.h
