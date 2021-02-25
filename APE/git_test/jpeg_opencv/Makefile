SOURCE :=main.cpp Jpeg2dimg.cpp
OBJS   :=main.o Jpeg2dimg.o
TARGET :=main

#CC      := /home/xu/VPcode/buildroot/dl/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc
CC      := g++
#LIBS    :=
#LDFLAGS := -L. -lfunc
DEFINES :=
INCLUDES:=.I /home/xu/jpeg 
CFLAGS  := -fPIC

#LIBS = $(SUBLIBS)  -L/opt/qt-4.7.1/lib -lQtGui -L/usr/local/tslib/lib -L/opt/qt-4.7.1/lib -lQtNetwork -lQtCore -lpthread -ljpeg 

all: $(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o main main.o Jpeg2dimg.o -L../opencv-3.4.1/lib`pkg-config --cflags --libs opencv`
	
#$(OBJS):$(SOURCE)
	#$(CC) -c -std=c++11 main.cpp Jpeg2dimg.cpp -fPIC -o main.o Jpeg2dimg.o
main.o: main.cpp Jpeg2dimg.h
	$(CC) -c main.cpp
Jpeg2dimg.o: Jpeg2dimg.cpp Jpeg2dimg.h
	$(CC) -c Jpeg2dimg.cpp

clean:
	rm -rf *.o $(TARGET)

