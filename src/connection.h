#ifndef _ARBITER_CONNECTION_H
#define _ARBITER_CONNECTION_H

#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <zmq.h>

#include "hash.h"


typedef struct
{
	void *socket;
	pthread_mutex_t *mutex;
	pthread_t *tid;
	hash_t *commands;

	hash_t *store;
} conn_t;

conn_t *conn_init(void*);
char *conn_process(conn_t*, char*);

#endif