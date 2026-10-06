SHELL = /bin/sh

CC = cc
CFLAGS = -Wall
LDFLAGS = -lncurses -ltinfo
SRC = ${wildcard *.c}
OBJ  = ${SRC:.c=.o}
PROGRAM = zippychip

.PHONY: all clean

all: ${PROGRAM}

%.o: %.c
	${CC} -c -o $@ $< ${CFLAGS}

${PROGRAM}: ${OBJ}
	${CC} -o $@ $^ ${CFLAGS} $(LDFLAGS)

clean:
	-rm -f ${PROGRAM} ${PROGRAM}.exe $(OBJ)