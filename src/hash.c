#include "hash.h"
#include <stdio.h>

static _h_size_t do_hash_func(void *, _h_size_t);
static bool hash_resize(hash_t *hash, bool grow);

hash_t *hash_init()
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	if(hash == NULL)
	{
		return NULL;
	}

	// Allocate our base hash
	hash->keys_count = 0;
	hash->keys_alloc = 256;
	hash->bucket = HASH_ALLOC(hash->keys_alloc);

	if(hash->bucket == NULL)
	{
		free(hash);
		return NULL;
	}

	return hash;
}

bool hash_resize(hash_t *hash, bool grow)
{
	_h_size_t len 	= 0;
	_h_size_t bsize = 0;

	if(grow)
	{
		bsize = hash->keys_count;
		len = hash->keys_alloc << 2;
	} else
	{
		if(len < hash->keys_count)
		{
			// we can't resize if we're trying to
			// resize to a smaller len than we have
			// keys
			return false;
		}

		len = hash->keys_alloc >> 2;
		bsize = len;
	}

	hash_elm_t **new_hash = HASH_ALLOC(len);

	while(bsize--)
	{
		hash_elm_t *it 	= hash->bucket[bsize];
		_h_size_t h		= do_hash_func(it->key, len);
		
		do
		{
			new_hash[h] = it;

			if(it->next)
			{
				it = it->next;
			}
		} while(it->next);
	}

	// Set the new bucket up
	free(*hash->bucket);
	*(hash->bucket) = *new_hash;
	
	return true;
}

void hash_free(hash_t *hash)
{
	free(hash->bucket);
	free(hash);
}

_h_size_t do_hash_func(void *key, _h_size_t max)
{
	_h_size_t *ptr = (_h_size_t*)key;
	_h_size_t len = sizeof(key);
	_h_size_t hash = *ptr * len;

	while(len--)
	{
		hash ^= hash*((*ptr<<2) + (len<<4));
	}

	return hash % max;
}

hash_elm_t *hash_add(hash_t *hash, void *key, void *value)
{
	// do we have room in our hash?
	if(hash->keys_count == hash->keys_alloc)
	{
		hash_resize(hash, true);
	}

	_h_size_t h = do_hash_func(key, hash->keys_alloc);

	hash_elm_t *element = (hash_elm_t*)calloc(1, sizeof(hash_elm_t));

	if(element == NULL)
	{
		return NULL;
	}

	element->key = malloc(sizeof(key));
	element->value = malloc(sizeof(value));

	memcpy(element->key, key, sizeof(key));
	element->value = value;

	element->next = NULL;

	if(hash->bucket[h] == NULL) // no collision
	{
		hash->bucket[h] = element;
		hash->keys_count++;
	} else // collision, add to end of linked list
	{
		hash_elm_t *it = hash->bucket[h];
		while(it->next)
		{
			if(sizeof(it->next->key) == sizeof(key) 
				&& memcmp(it->next->key, key, sizeof(key)) == 0)
			{
				// found the key in our existing map, replace
				// the value
				hash_elm_t *tmp = it->next;
				
				it->next = element;
				element->next = it->next->next;

				free(tmp);

				return element;
			}
			it = it->next;
		}
		
		// add it to the end of our bucket linked list
		it->next = element;
	}

	return element;
}

bool hash_remove(hash_t *hash, void *key)
{
	_h_size_t h = do_hash_func(key, hash->keys_alloc);

	if(hash->bucket[h] == NULL)
	{
		return false;
	} else
	{
		hash_elm_t *it   = hash->bucket[h];
		hash_elm_t *prev = hash->bucket[h];

		while(it)
		{
			if(sizeof(it->key) == sizeof(key) 
				&& memcmp(it->key, key, sizeof(key)) == 0)
			{
				if(prev == hash->bucket[h] && it->next)
				{
					hash->bucket[h] = it->next;
				} else if(it->next)
				{		
					prev->next = it->next;
					break;
				} else
				{
					prev->next = NULL;
				}
			}

			if(it->next)
			{
				prev = it;
				it = it->next;
			} else
			{
				it = NULL;
			}
		}
	}
	return false;
}

hash_elm_t *hash_find(hash_t *hash, void *key)
{
	_h_size_t h = do_hash_func(key, hash->keys_alloc);

	if(hash->bucket[h] == NULL)
	{
		return NULL;
	} else
	{
		hash_elm_t *it = hash->bucket[h];
		while(it)
		{
			if(sizeof(it->key) == sizeof(key) 
				&& memcmp(it->key, key, sizeof(key)) == 0)
			{
				return it;
			}

			if(it->next)
			{
				it = it->next;
			}
			else
			{ 
				// no element found, no next element (maybe first?)
				return NULL;
			}
		}
	}
	
	return NULL;
}