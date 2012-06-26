#ifndef _ARBITER_COMMANDS_H
#define _ARBITER_COMMANDS_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASH_ALLOC(size) (hash_elm_t**)calloc(size, sizeof(hash_elm_t *))
#define DEBUGS(msg) fprintf(stderr, "%s\n", msg);
#define DEBUGN(msg) fprintf(stderr, "%llu\n", msg);

typedef uint64_t _h_size_t;
typedef struct hash_elm_struct hash_elm_t;
typedef struct hash_struct hash_t;

struct hash_elm_struct
{
	void *key;
	void *value;
	hash_elm_t *next;
};

struct hash_struct
{
	hash_elm_t **bucket;
	_h_size_t keys_count;
	_h_size_t keys_alloc;
};

typedef struct
{
	void *elements;
} table_t;

hash_t *hash_init();
void hash_free(hash_t*);
hash_elm_t *hash_add(hash_t*, void*, void*);
hash_elm_t *hash_find(hash_t*, void*);
bool hash_remove(hash_t*, void*);

hash_t *hash_from_table(table_t*);

#endif