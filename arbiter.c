
#include <zmq.h>
#include <mongo.h>

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include "daemon.h"

void parse_opts(int, char**, bool*, char**, int*, char**, char**, int*);
void usage(const char*);

int main(int argc, char *argv[])
{
	bool fork;
	char *action = 0;
	int port = 1802;
	char *listen = "*";
	char *mongo_host = "127.0.0.1";
	int mongo_port = 27017;

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

	zmq_bind(zsock, "tcp://*:1802");

	mongo *conn = malloc(sizeof(mongo));
	mongo_init(conn);
	mongo_set_op_timeout(conn, 100);
	if(mongo_connect(conn, "127.0.0.1", 27017) == MONGO_ERROR)
	{
		// log conn->err and con->errstr
		exit(EXIT_FAILURE);
	}

	// We want writes not to fail
	mongo_write_concern write_concern[1];
	mongo_write_concern_init(write_concern);
	write_concern->w = 1;
	mongo_write_concern_finish(write_concern);

	while(true)
	{
	}

	mongo_destroy(conn);
	free(conn);
}

void parse_opts(int argc, char *argv[], bool *fork, char** action, 
					int* port, char** listen, char** mongo_host, 
					int* mongo_port)
{
	int opt;
	bool dostart = false;
	while((opt = getopt(argc, argv, "sf:p:l:m:h")) != -1)
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
			case 's':
				dostart = true;
				*fork = false;
				break;
			default:
				usage(argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	if(!dostart)
	{
		fprintf(stderr, "ERROR: -s or -f is required to start\n\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
}

void usage(const char *name)
{
	fprintf(stderr, "Usage:\n%s [-f start|stop] [-h] \n\n", name);
	fprintf(stderr, "where:\n");
	fprintf(stderr, " -h\t\tPrint this help.\n");
	fprintf(stderr, " -s\t\tDon't fork, and log to stderr\n");
	fprintf(stderr, " -f start|stop\tCreate a daemon.\n");
	fprintf(stderr, " -p\t\tPort number. (default: 1802)\n");
	fprintf(stderr, " -l\t\tListen address. (default: 0.0.0.0)\n");
	fprintf(stderr, " -m\t\tMongo host to connect to. (default: 127.0.0.1)\n");
	fprintf(stderr, " -n\t\tMongo port to connect to. (default: 27017)");
}
