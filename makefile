all: server client

server: server.o
	gcc -o server `xml2-config --cflags` server.o `xml2-config --libs` -lX11 -lglut -lGL
client: client.o
	gcc -o client `xml2-config --cflags` client.o `xml2-config --libs` -lpthread

server.o: server.c
	gcc -c -o server.o -I/usr/include/libxml2 server.c -lX11 -lglut -lGL
client.o: client.c
	gcc -std=c99 -c -o client.o -I/usr/include/libxml2 client.c

clean:
	rm -rf server client *.o

