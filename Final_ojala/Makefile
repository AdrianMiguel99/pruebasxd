CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -Iinclude -Ikernel

SRC := $(wildcard kernel/*.c)
OBJ := $(SRC:.c=.o)
TARGET := kernel_demo

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

clean:
	rm -f $(OBJ) $(TARGET)
