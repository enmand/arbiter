#include "hash.h"

static _h_size_t do_hash_func(const char *);
static bool hash_resize(hash_t *hash, bool grow);

hash_t *hash_init()
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	if(hash == NULL)
	{
		return NULL;
	}

	// Allocate our base hash
	hash->keys_alloc = 8;
	hash->bucket = HASH_ALLOC;

	if(hash->bucket == NULL)
	{
		free(hash);
		return NULL;
	}

	return hash;
}

void hash_free(hash_t *hash)
{
	_h_size_t i;
	for(i = 0; i < hash->keys_alloc; i++)
	{
		// clean our buckets
		while(hash->bucket[i])
		{
			free(hash->bucket[i]);
		}
	}

	free(hash);
}

_h_size_t do_hash_func(const char *key)
{
	_h_size_t hash = 0xd3fa;
	_h_size_t len = strlen(key);

	while(len--)
	{
		hash ^= ((hash << 5) + hash) + (*(key++)<<4);
	}

	return hash;
}

bool hash_add(hash_t *table, void *key, void *value, _h_size_t size)
{
	_h_size_t h = do_hash_func(key);
	return ~h;
}
