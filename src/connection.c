#include "connection.h"
#include "hash.h"

#include <stdio.h>

void func(){};

struct commands commandsTable[] = {
	{"get", func},
	{"set", func},
	{"ls", func},
};

conn_t *conn_init(void *socket)
{
	conn_t *conn = malloc(sizeof(conn_t));

	conn->socket = malloc(sizeof(void*));
	conn->mutex  = malloc(sizeof(pthread_mutex_t));
	conn->tid	 = malloc(sizeof(pthread_t));

	conn->socket = socket;

	return conn;
}

char* conn_process(conn_t* conn, char* msg)
{
	return msg;
}