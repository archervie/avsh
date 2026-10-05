CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2
TARGET = avsh

SRCS = avsh.c builtins.c exec.c utils.c
OBJS = $(SRCS:.c=.o)
HEADERS = avsh.h builtins.h exec.h utils.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

.PHONY: all clean
