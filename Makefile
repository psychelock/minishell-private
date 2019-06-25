CC=gcc
CFLAGS=-fsanitize=address  -Wall -Werror -Wextra -pedantic -std=c99 -g
SRC=${wildcard src/*.c}
OBJS=${SRC:.c=.o}
EXEC=minishell

.PHONY: all clean check

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

check:

clean:
	$(RM) $(OBJS) $(EXEC)
