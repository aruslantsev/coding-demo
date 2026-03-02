CC		:= cc
CFLAGS 	:= -Wall -Wextra -O2 -MMD

TARGET 	:= snake

SRC 	:= main.c snake.c terminal.c screen.c input.c
OBJ 	:= $(SRC:.c=.o)
DEP 	:= $(OBJ:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

-include $(DEP)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(DEP)

.PHONY: all clean
