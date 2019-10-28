CC = gcc
ARGS = -Wall

all: pingclient

server_c_udp: pingclient.c 
	$(CC) $(ARGS) -o pingclient  pingclient.c
clean:
	rm -f *.o pingclient *~
