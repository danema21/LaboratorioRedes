main: cliente.o server.o
	gcc cliente.o -o cliente
	gcc server.o -o server

cliente.o: cliente.c

server.o: server.c	

clean:
	rm -rf *.o cliente sistema
	clear
