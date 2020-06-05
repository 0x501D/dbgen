CC = gcc
CFLAGS = -Wall -Wextra -pedantic
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

dbgen: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o dbgen

.PHONY: clean
