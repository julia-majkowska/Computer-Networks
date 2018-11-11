# Julia Majkowska 290363
CC = gcc 
CFlags = -std=gnu99 -Wall -Wextra

all: traceroute.o icmp_receive.o icmp_send.o
	$(CC) $(CFlags) traceroute.o icmp_receive.o icmp_send.o -o traceroute

traceroute.o: traceroute.c icmp.h
	$(CC) $(CFlags) traceroute.c -c -o traceroute.o

icmp_receive.o: icmp_receive.c icmp.h
	$(CC) $(CFlags) icmp_receive.c -c -o icmp_receive.o

icmp_send.o: icmp_send.c icmp.h
	$(CC) $(CFlags) icmp_send.c -c -o icmp_send.o
  
clean:
	rm *.o 

distclean:
	rm *.o && rm traceroute