CC = g++
CFLAGS = -g -Wall
TARGET = asgn1
THREAD = -pthread
LIBS = pkg-config --cflags --libs opencv

all: $(TARGET)


$(TARGET): $(TARGET).cpp
	$(CC) $(TARGET).cpp -o $(TARGET) $(CFLAGS) $(THREAD) `$(LIBS)`


clean:
	$(RM) $(TARGET)