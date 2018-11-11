# Julia Majkowska 290363
CC = g++ 
CFlags = -std=gnu++11 -Wall -Wextra
all: router.o routing_ops.o io.o
	$(CC) $(CFlags) router.o routing_ops.o io.o -o router

router.o: router.cpp structures.h routing_ops.h
	$(CC) $(CFlags) router.cpp -c -o router.o

routing_ops.o: routing_ops.cpp structures.h routing_ops.h
	$(CC) $(CFlags) routing_ops.cpp -c -o routing_ops.o

io.o: io.cpp structures.h routing_ops.h
	$(CC) $(CFlags) io.cpp -c -o io.o

send: send.cpp structures.h
	$(CC) $(CFlags) send.cpp -o send
  
clean:
	rm *.o 

distclean:
	rm *.o && rm router