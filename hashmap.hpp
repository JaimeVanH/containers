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
		bool empty;
		K key;
		V value;
	};
	Vector<Bucket> _table;
	struct Iterator
	{
		// iterator will only ever point to a non-empty bucket
		Bucket *_bucket, *_bucket_end;
		Iterator operator++()
		{
			for (Bucket *next_bucket = (_bucket + 1); 
			     next_bucket < _bucket_end; 
			     ++next_bucket                       )
			{
				if (!next_bucket->empty)
					return {next_bucket, _bucket_end};
			}
			return {_bucket_end, _bucket_end};
		}
		Bucket& operator*() { return *_bucket; };
		bool operator!=(Iterator const &other) const { return _bucket != other._bucket; };
	};
};

template <typename K, typename V>
void init(HashMapLP<K, V> *self)
{
	init(&self->_table);
}

template <typename K, typename V>
void close(HashMapLP<K, V> *self)
{
	close(&self->_table);
}

template <typename K, typename V>
typename HashMapLP<K, V>::Iterator begin(HashMapLP<K, V> *self)
{
	using Bucket = typename HashMapLP<K, V>::Bucket;
	Bucket *bucket_end = &self->_table._data[self->_table._size];
	for (auto &bucket : &self->_table)
	{
		if (!bucket.empty)
		{
			return {&bucket, bucket_end};
		}
	}
	return {bucket_end, bucket_end};
}

template <typename K, typename V>
typename HashMapLP<K, V>::Iterator end(HashMapLP<K, V> *self)
{
	typename HashMapLP<K, V>::Bucket *bucket_end = &self->_table._data[self->_table._size];
	return {bucket_end, bucket_end};
}

template <typename K, typename V>
bool reserve(HashMapLP<K, V> *self, size_t capacity)
{
	if (self->_table._size < capacity)
	{
		HashMapLP<K, V> new_map;
		init(&new_map);
		if (resize(&new_map._table, capacity))
			return true;
		for (auto &bucket : &new_map._table)
		{
			bucket.empty = true;
		}
		for (auto &bucket : self)
		{
			*_probe(&new_map, bucket.key) = bucket; 
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
		if (tried_bucket->empty || tried_bucket->key == key)
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
			*bucket = Bucket{false, key, value};
			return false;
		}
		else
		{
			if (reserve(self, 1 + self->_table._size * 2))
				return true;
		}
	}
}

template <typename K, typename V>
V* get(HashMapLP<K, V> *self, K key)
{
	using Bucket = typename HashMapLP<K, V>::Bucket;
	Bucket *bucket = _probe(self, key);
	if (!bucket || bucket->empty)
		return nullptr;
	else
		return &bucket->value;
}

template <typename K, typename V>
bool remove(HashMapLP<K, V> *self, K key)
{
	using Bucket = typename HashMapLP<K, V>::Bucket;
	Bucket *bucket = _probe(self, key);
	if (!bucket || bucket->empty)
		return false;
	bucket->empty = true;
	return true;
}

void print(HashMapLP<int, int> *self)
{
	printf("Linear Probing Hashmap \n");
	printf("Current table size: %zu \n", self->_table._size);
	for (auto &bucket : &self->_table)
	{
		printf("Bucket: empty, key, value: %d, %d, %d\n", (int) bucket.empty, bucket.key, bucket.value);
	}
}