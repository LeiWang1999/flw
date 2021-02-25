SOURCE :=main.cpp
OBJS   :=main.o
TARGET :=main

CC      := g++
# g++ *.cpp -o test.out -L../opencv-3.4.0/lib`pkg-config --cflags --libs opencv    .cpp -o test.out -L../opencv-3.4.0/lib`pkg-config --cflags --libs opencv`
# LIBS    :=
# LDFLAGS := -L. -lfunc
DEFINES :=
INCLUDES:=.I 
CFLAGS  := -fPIC

all: $(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o main main.o -L../opencv-3.4.1/lib`pkg-config --cflags --libs opencv`
	
main.o: main.cpp
	$(CC) -c main.cpp 

clean:
	rm -rf *.o $(TARGET)

