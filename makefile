all: server.o client server

server.o: server.c
	gcc -c server.c -o obj/server.o

server: server.c
	gcc -o server obj/server.o -L. externals/tt_logger/tt_logger.a

client: client.c
	gcc client.c -o client

clean:
	rm -r client server obj/server.o
