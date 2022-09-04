.PHONY: all

CLIENT_TCP = client_TCP
CLIENT_UDP = client_UDP
SERVER = server

all: client_tcp client_udp server

client_tcp: $(CLIENT_TCP).o
	gcc $(CLIENT_TCP).o -o client_tcp

$(CLIENT_TCP).o: $(CLIENT_TCP).c
	gcc -c $(CLIENT_TCP).c

client_udp: $(CLIENT_UDP).o
	gcc $(CLIENT_UDP).o -o client_udp

$(CLIENT_UDP).o: $(CLIENT_UDP).c
	gcc -c $(CLIENT_UDP).c

server: $(SERVER).o
	gcc $(SERVER).o -o server

$(SERVER).o: $(SERVER).c
	gcc -c $(SERVER).c