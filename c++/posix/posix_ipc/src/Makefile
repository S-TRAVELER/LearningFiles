.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=01open 02unlink 03getattr 04send 04receive 05notify
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
01open:01open.o
	$(CC) $(CFLAGS) $^ -o $@ -lrt
02unlink:02unlink.c
	$(CC) $(CFLAGS) $^ -o $@ -lrt
03getattr:03getattr.c
	$(CC) $(CFLAGS) $^ -o $@ -lrt
04send:04send.c
	$(CC) $(CFLAGS) $^ -o $@ -lrt
04receive:04receive.c
	$(CC) $(CFLAGS) $^ -o $@ -lrt
05notify:05notify.c
	$(CC) $(CFLAGS) $^ -o $@ -lrt
clean:
	rm -f *.o $(BIN)
