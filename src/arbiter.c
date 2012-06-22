#include <zmq.h>

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include <getopt.h>

#if __APPLE__ || __linux__ || __sun__
#define CPU_SIZE sysconf( _SC_NPROCESSORS_ONLN )
#else
#define CPU_SIZE 1l
#endif

#include "daemon.h"

void parse_opts(int, char**, bool*, char**, char**, char**, char**, char**);
void usage(const char*);

static void _mainloop(void *);

void *something(void *arg)
{
	fprintf(stderr, "something");
	void *ret;
	return ret;
}

int main(int argc, char *argv[])
{
	bool fork;
	char *action = 0;
	char *port = "1802";
	char *listen = "*";
	char *mongo_host = "127.0.0.1";
	char *mongo_port = "27017";

	// get our opts to run our program
	parse_opts(argc, argv, &fork, &action, &port, &listen, &mongo_host, &mongo_port);

	if(fork)
	{
		if(strcmp(action, "start") == 0)
		{
			daemonize(ARBITER_START);
		} else 
		{
			daemonize(ARBITER_STOP);
		}
	}

	// set up zmq
	void *zctx  = zmq_init(CPU_SIZE);
	void *zsock = zmq_socket(zctx, ZMQ_REP);

	char *zmqhost = malloc(sizeof(zmqhost) + strlen(listen) + 1);
	sprintf(zmqhost, "tcp://%s:%s", listen, port);
	zmq_bind(zsock, zmqhost);


	_mainloop(zsock);
}

struct worker
{
	pthread_mutex_t mutex;
	pthread_t tid;
	pthread_attr_t attr;
};

void _mainloop(void *zsock)
{
	while(true)
	{
		zmq_msg_t *zmsg = malloc(sizeof(zmq_msg_t));
		zmq_msg_init(zmsg);

		int rc = zmq_recv(zsock, zmsg, 0);

		int size = zmq_msg_size(zmsg);
		char *_msg = malloc(size + 1);
		memcpy(_msg, zmq_msg_data(zmsg), size);

		if(rc == 0) // zmq_recv 0 is non-error
		{
			fprintf(stderr, "Got a connection\n");
		}

		zmq_send(zsock, zmsg, 0);
		zmq_msg_close(zmsg);

		free(_msg);
		usleep(200);
	}
}

void parse_opts(int argc, char *argv[], bool *fork, char** action, 
					char** port, char** listen, char** mongo_host, 
					char** mongo_port)
{
	int opt;
	bool dostart = false;
	while((opt = getopt(argc, argv, "vf:p:l:m:n:h")) != -1)
	{
		switch(opt)
		{
			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
			case 'f':
				dostart = true;
				*fork = true;
				*action = optarg;
				break;
			case 'v':
				dostart = true;
				*fork = false;
				break;
			case 'l':
				*listen = optarg;
				break;
			case 'p':
				*port = optarg;
				break;
			case 'm':
				*mongo_host = optarg;
				break;
			case 'n':
				*mongo_port = optarg;
				break;
			default:
				usage(argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	if( ! dostart)
	{
		fprintf(stderr, "ERROR: -s or -f is required to start\n\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
}

void usage(const char *name)
{
	fprintf(stderr, "Usage:\n%s [-f start|stop] [-h] [-v] [-l address] [-p port] \n\n", name);
	fprintf(stderr, "where:\n");
	fprintf(stderr, " -h\t\tPrint this help.\n");
	fprintf(stderr, " -v\t\tDon't fork, and log to stderr\n");
	fprintf(stderr, " -f start|stop\tCreate a daemon.\n");
	fprintf(stderr, " -p\t\tPort number. (default: 1802)\n");
	fprintf(stderr, " -l\t\tListen address. (default: 0.0.0.0)\n");
}
