CC=gcc
CFLAGS=-g -Wall
TARGET=LEA
OBJS=LEA.o
 
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)
	rm $(OBJS)

clean:
	rm $(OBJECT) $(TARGET)

