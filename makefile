all: server client_rad client_obst client_move

server: server.o
	gcc -o server `xml2-config --cflags` server.o `xml2-config --libs` -lX11 -lglut -lGL
client_rad: client_rad.o
	gcc -o client_rad `xml2-config --cflags` client_rad.o `xml2-config --libs` -lpthread
client_obst: client_obst.o
	gcc -o client_obst `xml2-config --cflags` client_obst.o `xml2-config --libs` -lpthread
client_move: client_move.o
	gcc -o client_move `xml2-config --cflags` client_move.o `xml2-config --libs` -lpthread

server.o: server.c
	gcc -c -o server.o -I/usr/include/libxml2 server.c -lX11 -lglut -lGL
client_rad.o: client_rad.c
	gcc -std=c99 -c -o client_rad.o -I/usr/include/libxml2 client_rad.c
client_obst.o: client_obst.c
	gcc -std=c99 -c -o client_obst.o -I/usr/include/libxml2 client_obst.c
client_move.o: client_move.c
	gcc -std=c99 -c -o client_move.o -I/usr/include/libxml2 client_move.c

clean:
	rm -rf server client_rad client_obst client_move *.o

