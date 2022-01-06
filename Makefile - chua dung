CC=gcc
CFLAGS=-I.

all: server client

server: state.o poll-echo-server.o server_handle_message.o server-message.o
	$(CC) -o server state.o poll-echo-server.o server_handle_message.o server-message.o -I.

client: client-message.o tcp_client.o
	$(CC) -o client client-message.o tcp_client.o -I.

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 