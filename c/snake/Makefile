CC = cc
CFLAGS = -Wall -Wextra -march=x86-64 -O2

TARGET = snake
SRC = main.c snake.c terminal.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
