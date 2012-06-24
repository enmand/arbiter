#ifndef _ARBITER_COMMANDS_H
#define _ARBITER_COMMANDS_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef uint64_t _h_size_t;
#define HASH_ALLOC (hash_elm_t**)calloc(hash->keys_alloc, sizeof(hash_elm_t))
#define HASH(key) do_hash_func(key)

typedef struct
{
	void *key;
	void *value;
	struct hash_elm_t *next;
} hash_elm_t;

typedef struct
{
	hash_elm_t **bucket;
	_h_size_t keys_count;
	_h_size_t keys_alloc;
} hash_t;

hash_t *hash_init();
void hash_free(hash_t*);
bool hash_add(hash_t*, void*, void*, _h_size_t);
void *hash_find(hash_t*, void*);
bool hash_remove(hash_t*, void*);

#endif