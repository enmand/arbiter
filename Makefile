all: clean .c.so main

main:
	gcc -Wall -std=c99 -I/opt/local/include -L/opt/local/lib -L. -ldaemon -lzmq -lmongoc arbiter.c -o arbiter

.c.so:
	gcc -Wall -c -fPIC -o daemon.o daemon.c
	gcc -shared -Wl,-install_name,libdaemon.so -o libdaemon.so daemon.o
	rm daemon.o

clean:
	rm -f *.so
	rm -f arbiter
	rm -f arbiter.pid
