CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -std=c99 -g -D _POSIX_C_SOURCE=200809L
SRC=${wildcard src/*.c}
OBJS=${SRC:.c=.o}
EXEC=minishell

.PHONY: all clean check

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

check:

clean:
	$(RM) $(OBJS) $(EXEC)
