all: server client_rad client_obst client_move client_proc

server: server.o
	gcc -o server `xml2-config --cflags` server.o `xml2-config --libs`
client_rad: client_rad.o
	gcc -o client_rad  `xml2-config --cflags` client_rad.o  `xml2-config --libs` -lpthread -lm 
client_obst: client_obst.o
	gcc -o client_obst `xml2-config --cflags` client_obst.o `xml2-config --libs` -lpthread -lm
client_move: client_move.o
	gcc -o client_move `xml2-config --cflags` client_move.o `xml2-config --libs` -lpthread -lm -lX11 -lglut -lGL 
client_proc: client_proc.o
	gcc -o client_proc `xml2-config --cflags` client_proc.o `xml2-config --libs` -lpthread -lm

server.o: server.c
	gcc -c -o server.o -I/usr/include/libxml2 server.c
client_rad.o: client_rad.c
	gcc -std=gnu99 -c -I/usr/include/libxml2 client_rad.c  -lpthread
client_obst.o: client_obst.c
	gcc -std=gnu99 -c -I/usr/include/libxml2 client_obst.c -lpthread
client_move.o: client_move.c
	gcc -std=gnu99 -c -I/usr/include/libxml2 client_move.c -lpthread -lX11 -lglut -lGL
client_proc.o: client_proc.c
	gcc -std=gnu99 -c -I/usr/include/libxml2 client_proc.c -lpthread

clean:
	rm -rf server client_rad client_obst client_move client_proc *.o ask*.xml reader*.xml *?Data.xml
