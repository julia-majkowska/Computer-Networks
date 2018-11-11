# Julia Majkowska 290363
CC = g++ 
CFlags = -std=gnu++11 -Wall -Wextra
all: server.o sockwrap.o server_ops.o err.o
	$(CC) $(CFlags) server.o sockwrap.o server_ops.o err.o -o server

server.o: server.cpp sockwrap.h server_ops.h
	$(CC) $(CFlags) server.cpp -c -o server.o

sockwrap.o: sockwrap.cpp sockwrap.h
	$(CC) $(CFlags) sockwrap.cpp -c -o sockwrap.o
	
server_ops.o: server_ops.cpp server_ops.h
	$(CC) $(CFlags) server_ops.cpp -c -o server_ops.o
	
err.o: err.cpp server_ops.h
	$(CC) $(CFlags) err.cpp -c -o err.o
clean:
	rm *.o 

distclean:
	rm *.o && rm server