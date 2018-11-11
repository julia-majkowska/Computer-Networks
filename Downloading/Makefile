# Julia Majkowska 290363
CC = g++ 
CFlags = -std=gnu++11 -Wall -Wextra
all: transport.o transport_ops.o
	$(CC) $(CFlags) transport.o transport_ops.o  -o transport

transport.o: transport.cpp transport_ops.h
	$(CC) $(CFlags) transport.cpp -c -o transport.o

transport_ops.o: transport_ops.cpp transport_ops.h
	$(CC) $(CFlags) transport_ops.cpp -c -o transport_ops.o
  
clean:
	rm *.o 

distclean:
	rm *.o && rm transport