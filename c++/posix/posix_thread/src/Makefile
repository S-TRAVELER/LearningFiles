.PHONY:clean all
CC=gcc
CCFLAGS=-Wall -g
BIN=01create 02commu_socket 02commu_client 03attr 04cond_var
all:$(BIN)
%.o:%.c
	$(CC) $(CCFLAGS) -c $< -o $@
01create:01create.o
	$(CC) $(CCFALGS) $^ -o $@ -lpthread
02commu_socket:02commu_socket.o
	$(CC) $(CCFALGS) $^ -o $@ -lpthread
02commu_client:02commu_client.o
	$(CC) $(CCFALGS) $^ -o $@ -lpthread
03attr:03attr.o
	$(CC) $(CCFALGS) $^ -o $@ -lpthread
04cond_var:04cond_var.o
	$(CC) $(CCFALGS) $^ -o $@ -lpthread


clean:
	rm -f *.o $(BIN)
