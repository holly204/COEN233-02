all: clean client server

client:
	gcc UDPclient02.c -g -o client

server:
	gcc UDPserver02.c -g -o server

clean:
	rm -rf ./client ./server
