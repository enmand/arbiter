all: clean .objects main

main:
	gcc -Wall -O3 -std=c99 -I/opt/local/include -L/opt/local/lib -pthread -L. -D_REENTRANT -lzmq connection.o daemon.o node.o arbiter.c -o arbiter

.objects:
	gcc -Wall -O3 -c -fPIC -o daemon.o daemon.c
	gcc -I/opt/local/include -L/opt/local/lib  -Wall -O3 -c -fPIC -pthread -o connection.o connection.c
	gcc -Wall -O3 -c -fPIC -o logging.o logging.c
	gcc -Wall -O3 -c -fPIC -o node.o node.c

clean:
	rm -f *.so *.o
	rm -f arbiter