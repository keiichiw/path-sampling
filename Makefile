CC=g++
FLGS=-Wall -Wextra -g -O3 -std=c++11 #-DNDEBUG
LIB=~/lib/boost_1_58_0
OBJS=util.o graph.o
TARGET=main
all: $(TARGET)

$(TARGET): main.cpp $(OBJS)
	$(CC) $(FLGS) $(OBJS) -I $(LIB) main.cpp -o $(TARGET)

.cpp.o:
	$(CC) -c $< $(FLGS) -I $(LIB)

clean:
	rm -f main *~ *.o
