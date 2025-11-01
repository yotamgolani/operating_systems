CC = gcc
CFLAGS = -g -O3 -Wall -Wextra -std=c11

# Define the name of the executable
TARGET = copy

# Define source file
SRC = copy.c

# Default target: builds the executable directly
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean target: removes the executable
clean:
	rm -f $(TARGET)