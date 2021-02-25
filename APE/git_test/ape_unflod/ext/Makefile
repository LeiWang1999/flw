SOURCE :=main.cpp
OBJS   :=main.o
TARGET :=main

CC      := /home/xu/VPcode/buildroot/dl/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc
#LIBS    :=
#LDFLAGS := -L. -lfunc
DEFINES :=
INCLUDE := -I.
CFLAGS  := -fPIC

#LIBS = $(SUBLIBS)  -L/opt/qt-4.7.1/lib -lQtGui -L/usr/local/tslib/lib -L/opt/qt-4.7.1/lib -lQtNetwork -lQtCore -lpthread -ljpeg 

all: $(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o main main.o -L /home/xu/jpeg -ljpeg  -lstdc++ -lm
	
$(OBJS):$(SOURCE)
	$(CC) -c  -std=c++11 main.cpp -fPIC -o main.o
clean:
	rm -rf *.o $(TARGET)






