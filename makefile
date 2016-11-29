all: server client

server: server.o
	gcc -o server server.o
client: client.o
	gcc -o client `xml2-config --cflags` client.o `xml2-config --libs`

server.o: server.c
	gcc -c -o server.o server.c
client.o: client.c
	gcc -c -o client.o -I/usr/include/libxml2 client.c

clean:
	rm -rf server client *.o

