CC = g++
CFLAGS = -g -Wall -Weffc++ -std=c++11 -Iinclude
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
EXEC = simulation

# Build the executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Compile .cpp to .o
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(EXEC)

# Run the program
run: $(EXEC)
	./$(EXEC) config.txt

.PHONY: clean run
