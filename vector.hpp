#include <cstddef>
#include <cstdlib>
#include <cstdio>

constexpr bool debug = true;
void debugPrint(char const *str)
{
	if constexpr (debug)
		puts(str);
}

template <typename T>
struct Vector
{
	T *_data;
	size_t _size;
	size_t _capacity;
};

template <typename T>
void init(Vector<T> *self)
{	
	self->_data = nullptr;
	self->_size = 0;
	self->_capacity = 0;
}

template <typename T>
void close(Vector<T> *self)
{
	free(self->_data);
}

template <typename T>
T* get(Vector<T> *self, size_t pos)
{
	return &self->_data[pos];
}

template <typename T>
T* sget(Vector<T> *self, size_t pos)
{
	if (pos > self->_size)
		return nullptr;
	else
		return &self->_data[pos];
}


template <typename T>
size_t size(Vector<T> *self)
{
	return self->_size;
}

template <typename T>
size_t capacity(Vector<T> *self)
{
	return self->_capacity;
}

template <typename T>
bool resize(Vector<T> *self, size_t size)
{
	if (self->_capacity < size)
	{
		if (reserve(self, size))
			return true;
	}
	self->_size = size;
	return false;
}

template <typename T>
bool reserve(Vector<T> *self, size_t capacity)
{
	if (self->_capacity < capacity)
	{
		T *new_data = (T*) realloc(self->_data, capacity * sizeof *self->_data);
		if (!new_data)
			return true;
		self->_data = new_data;
		self->_capacity = capacity;
	}
	return false;
}

template <typename T>
bool push(Vector<T> *self, T elem)
{
	if (self->_size = self->_capacity)
		if (reserve(self, self->_capacity * 2))
			return true;
	self->_data[self->_size] = elem;
	++self->_size;
	return false;
}

template <typename T>
T pop(Vector<T> *self)
{
	--self->_size;
	return self->_data[self->_size];
}

template <typename T>
bool insert(Vector<T> *self, T elem, size_t insert_pos)
{
	if (self->_size = self->_capacity)
		if (reserve(self, self->_capacity * 2))
			return true;
	for (size_t pos = self->_size; pos > insert_pos; ++pos)
	{
		self->_data[pos] = self->_data[pos-1];
	}
	self->_data[insert_pos] = elem;
	++self->_size;
}

template <typename T>
bool compact(Vector<T> *self)
{
	T *new_data = (T*) realloc(self->_data, self->_size * sizeof *self->_data);
	if (!new_data)
		return true;
	self->data = new_data;
	self->_capacity = self->_size;
	return false;
}

template <typename T>
T* begin(Vector<T> *self)
{
	return self->_data;
}

template <typename T>
T* end(Vector<T> *self)
{
	return self->_data + self->_size;
}
