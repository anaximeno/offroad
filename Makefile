CC = gcc
CFLAGS = -Wall -fPIC -g
OUT = offroad

SRC_FILES = src/argparser.c src/common.c src/offroad.c src/pnode.c src/rnode.c
OBJS = argparser.o common.o offroad.o pnode.o rnode.o

AX_C_COMMON_FILES = include/ax-c-common/src/ax-log.c \
					include/ax-c-common/src/ax-result.c
AX_C_COMMON_OBJS = ax-log.o ax-result.o


$(OUT): main.c $(OBJS) $(AX_C_COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJS): $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -c

$(AX_C_COMMON_OBJS): $(AX_C_COMMON_FILES)
	$(CC) $(CFLAGS) $^ -c

clean:
	rm *.o $(OUT)
