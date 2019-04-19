.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=01open 02unlink 03write 03read
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
01open:01open.o
	$(CC) $(CFLAGS) $^ -o $@ -lrt
02unlink:02unlink.o
	$(CC) $(CFLAGS) $^ -o $@ -lrt
03write:03write.o
	$(CC) $(CFLAGS) $^ -o $@ -lrt
03read:03read.o
	$(CC) $(CFLAGS) $^ -o $@ -lrt
clean:
	rm -f *.o $(BIN)
