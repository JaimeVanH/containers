#include <cstddef>
#include <cstdio>

#include "vector.hpp"

template <typename T>
size_t hash(T);

template <typename K, typename V>
struct HashMapLP
{
	struct Bucket
	{
		enum State : unsigned char
		{
			EMPTY = 0, 
			FULL = 1, 
			TERMINATOR = 2
		};
		State state;
		K key;
		V value;
	};
	Vector<Bucket> _table;
};

template <typename K, typename V>
bool init(HashMapLP<K, V> *self)
{
	init(&self->_table);
	if (resize(&self->_table, 1))
		return true;
	self->_table._data[0].state = HashMapLP<K, V>::Bucket::TERMINATOR;
}

template <typename K, typename V>
void close(HashMapLP<K, V> *self)
{
	close(&self->_table);
}


template <typename K, typename V>
typename HashMapLP<K, V>::Bucket* next(typename HashMapLP<K, V>::Bucket* iterator)
{
	++iterator;
	while (!iterator->state)
		++iterator;
	return iterator;
}

template <typename K, typename V>
typename HashMapLP<K, V>::Bucket* begin(HashMapLP<K, V> *self)
{
	return next<K, V>(self->_table._data - 1);
}

template <typename K, typename V>
typename HashMapLP<K, V>::Bucket* end(HashMapLP<K, V> *self)
{
	return &self->_table._data[self->_table._size-1];
}

template <typename K, typename V>
bool reserve(HashMapLP<K, V> *self, size_t capacity)
{
	using Bucket = typename HashMapLP<K, V>::Bucket;
	if (self->_table._size - 1 < capacity)
	{
		HashMapLP<K, V> new_map;
		init(&new_map);
		if (resize(&new_map._table, capacity + 1))
			return true;
		for (Bucket *bucket = begin(&new_map._table);
		     bucket != end(&new_map._table)-1;
		     ++bucket                              )
		{
			bucket->state = Bucket::EMPTY;
		}
		end(&new_map)->state = Bucket::TERMINATOR;
		for (Bucket *bucket = begin(self);
		     bucket != end(self);
		     bucket = next<K, V>(bucket)        )
		{
			*_probe(&new_map, bucket->key) = *bucket; 
		}
		close(self);
		*self = new_map;
	}
	return false;	
}

template <typename K, typename V>
typename HashMapLP<K, V>::Bucket* _probe(HashMapLP<K, V> *self, K key)
{
	using Bucket = typename HashMapLP<K, V>::Bucket;
	size_t key_hash = hash(key) % self->_table._size;
	size_t tried_bucket_offset = key_hash;
	bool tried_first_bucket = false;
	while (key_hash != tried_bucket_offset || !tried_first_bucket)
	{	
		Bucket *tried_bucket = get(&self->_table, tried_bucket_offset);
		if (!tried_bucket->state || tried_bucket->key == key)
		{
			return tried_bucket;
		}
		tried_bucket_offset = (tried_bucket_offset + 1) % self->_table._size;
		tried_first_bucket = true;
	}
	return nullptr;
}


template <typename K, typename V>
bool put(HashMapLP<K, V> *self, K key, V value)
{
	using Bucket = typename HashMapLP<K, V>::Bucket;
	for (;;)
	{
		Bucket *bucket = _probe(self, key);
		if (bucket)
		{
			*bucket = Bucket{Bucket::FULL, key, value};
			return false;
		}
		else
		{
			if (reserve(self, self->_table._size * 2))
				return true;
		}
	}
}

template <typename K, typename V>
V* get(HashMapLP<K, V> *self, K key)
{
	using Bucket = typename HashMapLP<K, V>::Bucket;
	Bucket *bucket = _probe(self, key);
	if (!bucket || bucket->state)
		return nullptr;
	else
		return &bucket->value;
}

template <typename K, typename V>
bool remove(HashMapLP<K, V> *self, K key)
{
	using Bucket = typename HashMapLP<K, V>::Bucket;
	Bucket *bucket = _probe(self, key);
	if (!bucket || bucket->state)
		return false;
	bucket->state = Bucket::EMPTY;
	return true;
}

void print(HashMapLP<int, int> *self)
{
	printf("Linear Probing Hashmap \n");
	printf("Current table size: %zu \n", self->_table._size);
	for (auto &bucket : &self->_table)
	{
		printf("Bucket: state, key, value: %d, %d, %d\n", bucket.state, bucket.key, bucket.value);
	}
}