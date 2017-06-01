hw1: hw1.o util.o
	gcc -o hw1 -g hw1.o util.o -lcrypto -lm

hw1.o: hw1.c 
	gcc -g -c -Wall hw1.c 

util.o: util.c util.h
	gcc -g -c -Wall util.c
clean:
	rm -f *.o hw1
