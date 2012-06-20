#include <zmq.h>

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include "daemon.h"

void parse_opts(int, char**, bool*, char**, char**, char**, char**, char**);
void usage(const char*);

int main(int argc, char *argv[])
{
	bool fork;
	char *action = 0;
	char *port = "1802";
	char *listen = "*";
	char *mongo_host = "127.0.0.1";
	char *mongo_port = "27017";

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

	void *zctx  = zmq_init(2);
	void *zsock = zmq_socket(zctx, ZMQ_REP);

	char *zmqhost = malloc(sizeof(zmqhost) + strlen(listen) + 1);
	sprintf(zmqhost, "tcp://%s:%s", listen, port);
	zmq_bind(zsock, zmqhost);



	while(true)
	{
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
