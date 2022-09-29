all: server client

server: server.o
	gcc server.o -o server

client: client.o game.o
	gcc client.o game.o -o client

server.o: server.c server.h
	gcc -c server.c

client.o: client.c game.c client.h game.h
	gcc -c client.c game.c

game.o: game.c game.h
	gcc -c game.c 

clean:
	rm *.o server client