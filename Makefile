CC = gcc
CFLAGS = -Wall -fPIC -g
OUT = offroad

SRC_FILES = src/argparser.c src/offroad.c src/pnode.c src/rnode.c
OBJS = argparser.o offroad.o pnode.o rnode.o

AX_C_COMMON_PATH = ax-c-common
AX_C_COMMON_OBJS = $(AX_C_COMMON_PATH)/ax-log.o \
				   $(AX_C_COMMON_PATH)/ax-result.o


$(OUT): main.c $(OBJS) $(AX_C_COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJS): $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -c

$(AX_C_COMMON_OBJS): $(AX_C_COMMON_PATH)
	cd $(AX_C_COMMON_PATH) && $(MAKE)

clean:
	-cd $(AX_C_COMMON_PATH) && $(MAKE) clean
	-rm *.o $(OUT)
