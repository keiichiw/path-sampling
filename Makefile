CC=g++
FLGS=-Wall -Wextra -g -O3 -std=c++11 -DNDEBUG
OBJS=graph.o
TARGET=main
all: $(TARGET)

$(TARGET): main.cpp $(OBJS)
	$(CC) $(FLGS) $(OBJS) main.cpp -o $(TARGET)

.cpp.o:
	$(CC) -c $< $(FLGS)

clean:
	rm -f main *~ *.o
