CC = g++
CFLAGS = -g -Wall
TARGET = asgn1_sub2
THREAD = -pthread
LIBS = pkg-config --cflags --libs opencv

all: $(TARGET)


$(TARGET): $(TARGET).cpp
	$(CC) $(TARGET).cpp warp_crop.cpp queue.cpp dynamic.cpp -o $(TARGET) $(CFLAGS) $(THREAD) `$(LIBS)`


method4: method4.cpp
	$(CC) method4.cpp warp_crop.cpp queue.cpp dynamic.cpp -o method4 $(CFLAGS) $(THREAD) `$(LIBS)`


method3: method3.cpp
	$(CC) method3.cpp warp_crop.cpp queue.cpp dynamic.cpp -o method3 $(CFLAGS) $(THREAD) `$(LIBS)`


method2: method2.cpp
	$(CC) method2.cpp warp_crop.cpp queue.cpp dynamic.cpp -o method2 $(CFLAGS) $(THREAD) `$(LIBS)`


method1: method1.cpp
	$(CC) method1.cpp warp_crop.cpp queue.cpp dynamic.cpp -o method1 $(CFLAGS) $(THREAD) `$(LIBS)`


asgn1_sub1: asgn1_sub1.cpp
	$(CC) asgn1_sub1.cpp warp_crop.cpp -o asgn1_sub1 $(CFLAGS) $(THREAD) `$(LIBS)`


clean:
	$(RM) $(TARGET)
	$(RM) method4
	$(RM) method3
	$(RM) method2
	$(RM) method1
	$(RM) asgn1_sub1