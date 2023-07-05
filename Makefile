CC = gcc
CFLAGS = -Wall -fPIC -g

SRC_FILES = src/argparser.c src/common.c
OBJS = argparser.o common.o

main: main.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJS): $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -c

clean:
	rm *.o main
