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

	conn->commands = hash_init();
	hash_add(conn->commands, "get", "get");
	hash_add(conn->commands, "set", "set");
	hash_add(conn->commands, "ls", "ls");
	hash_add(conn->commands, "rm", "rm");
	hash_add(conn->commands, "quit", "quit");

	conn->store = hash_init();

	return conn;
}

char* conn_process(conn_t* conn, char* msg)
{
	hash_elm_t *el = hash_find(conn->commands, msg);
	if(el == NULL)
	{
		return NULL;
	}

	return el->value;
}