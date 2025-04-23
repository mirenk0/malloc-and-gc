CC = gcc
CFLAGS = -Wall -Wextra -g
SRC = malloc.c main.c
OBJ = $(SRC:.c=.o)
TARGET = test_malloc

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean

