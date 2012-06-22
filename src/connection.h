#ifndef _ARBITER_CONNECTION_H
#define _ARBITER_CONNECTION_H

#include <stdlib.h>

#include <pthread.h>
#include <zmq.h>

struct commands 
{
	char *name;
	void *proc;
	char *alias;
};

typedef struct 
{
	void *socket;
	pthread_mutex_t *mutex;
	pthread_t *tid;
} conn_t;

conn_t *conn_init(void*);
char *conn_process(conn_t*, char*);

#endif