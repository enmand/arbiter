
#include <zmq.h>
#include <mongo.h>
#include <stdbool.h>

#include "daemon.h"


int main()
{
	daemonize();

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
		/*bson b[1];
		bson_init(b);

		bson_append_string(b, "msg", "test");
		bson_finish(b);

		if(mongo_insert(conn, "testing.something", b, write_concern) == MONGO_ERROR)
		{
			// log conn->err and con->errstr
			exit(EXIT_FAILURE);
		}

		bson_destroy(b);*/
	}

	mongo_destroy(conn);
	free(conn);
}