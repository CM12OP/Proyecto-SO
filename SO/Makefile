CC = gcc
CFLAGS = -pthread -Wall -Wextra -g
SRC = main.c clock.c timer.c process_generator.c scheduler.c machine.c process_queue.c
OBJ = $(SRC:.c=.o)
TARGET = kernel_simulator

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
