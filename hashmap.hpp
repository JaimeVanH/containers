#include <cstddef>
#include <optional>

#include "vector.hpp"

template <typename T>
size_t hash(T);

template <typename K, typename V>
struct HashMap
{
	struct KVPair
	{
		K key;
		V value;
	};
	Vector<std::optional<KVPair>> _table;
	
};

template <typename K, typename V>
bool init(HashMap<K, V> *self, size_t size)
{
	using KVPair = typename HashMap<K, V>::KVPair;
	init(&self->_table);
	if (resize(&self->_table, size))
		return true;
	for (auto &cell : &self->_table)
		cell = std::nullopt;
	return false;
}

template <typename K, typename V>
void close(HashMap<K, V> *self)
{
	close(&self->_table);
}

template <typename K, typename V>
bool put(HashMap<K, V> *self, K key, V value)
{
	using KVPair = typename HashMap<K, V>::KVPair;
	size_t key_hash = hash(key) % size(&self->_table);
	size_t cell_try = key_hash;
	bool tried = false;
	while (key_hash != cell_try || !tried)
	{
		std::optional<KVPair> *cell = get(&self->_table, cell_try);
		if (!*cell || (**cell).key == key)
		{
			*cell = KVPair{key, value};
			return false;
		}
		cell_try = (cell_try + 1) % size(&self->_table);
		tried = true;
	}
	return true;
}

template <typename K, typename V>
V* get(HashMap<K, V> *self, K key)
{
	using KVPair = typename HashMap<K, V>::KVPair;
	size_t key_hash = hash(key) % size(&self->_table);
	size_t cell_try = key_hash;
	bool tried = false;
	while (key_hash != cell_try || !tried)
	{
		std::optional<KVPair> *cell = get(&self->_table, cell_try);
		if (!*cell)
		{
			return nullptr;
		}
		if ((**cell).key == key)
		{
			return &(**cell).value;
		}
		cell_try = (cell_try + 1) % size(&self->_table);
		tried = true;
	}
	return nullptr;
}

template <typename K, typename V>
bool remove(HashMap<K, V> *self, K key)
{
	using std::nullopt;
	using KVPair = typename HashMap<K, V>::KVPair;
	size_t key_hash = hash(key) % size(&self->_table);
	size_t cell_try = key_hash;
	bool tried = false;
	while (key_hash != cell_try || !tried)
	{
		std::optional<KVPair> *cell = get(&self->_table, cell_try);
		if (!*cell)
		{
			return false;
		}
		if ((**cell).key == key)
		{
			*cell = nullopt;
			return true;
		}
		cell_try = (cell_try + 1) % size(&self->_table);
		tried = true;
	}
	return false;
}