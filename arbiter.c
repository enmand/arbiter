
#include <zmq.h>
#include <mongo.h>
#include <stdbool.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "daemon.h"


int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage:\n%s start|stop\n\n", argv[0]);
		fprintf(stderr, "\tstart: start execution of the daemon\n");
		fprintf(stderr, "\tstop:  stop execution of the daemon\n\n");
		exit(EXIT_FAILURE);
	}
	if(strcmp(argv[1], "start") == 0)
	{
		daemonize(ARBITER_START);
	} else 
	{
		daemonize(ARBITER_STOP);
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
