# Compiler
CC = g++

CFLAGS = -Wall -Wextra -pedantic -std=c++11

OBJECTS = UUniFast.o

TARGET = UUniFast

# make object files
%.o: %.cpp
	$(CC) $(CFLAGS) $*.cpp -c -o $@

# main executable
exec: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o $(TARGET)