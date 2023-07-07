CC = gcc
CFLAGS = -Wall -fPIC -g
OUT = offroad

SRC_FILES = src/argparser.c src/common.c src/offroad.c src/pnode.c src/rnode.c
OBJS = argparser.o common.o offroad.o pnode.o rnode.o

AX_C_COMMON_OBJS = include/ax-c-common/ax-log.o include/ax-c-common/ax-result.o

$(OUT): main.c $(OBJS) $(AX_C_COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJS): $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -c

$(AX_C_COMMON_OBJS):
	cd include/ax-c-common/ && $(MAKE)

clean:
	-cd include/ax-c-common/ && $(MAKE) clean
	-rm *.o $(OUT)
