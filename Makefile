CC=clang++
FLGS=-Wall -Wextra -g -O3 -std=c++11
LIB=boost_unit_test_framework-mt
OBJS=util.o graph.o
TARGET=main
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLGS) $(OBJS) main.cpp -o $(TARGET)

.cpp.o:
	$(CC) -c $< $(FLGS)

clean:
	rm -f main *~ *.o
