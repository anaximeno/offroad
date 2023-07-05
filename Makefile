CC = gcc
CFLAGS = -Wall -fPIC -g

SRC_FILES = src/argparser.c src/common.c src/offroad.c src/pnode.c src/rnode.c
OBJS = argparser.o common.o offroad.o pnode.o rnode.o

OUT = offroad

$(OUT): main.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJS): $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -c

clean:
	rm *.o $(OUT)
