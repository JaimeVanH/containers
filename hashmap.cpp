#include <cstdio>

#include "hashmap.hpp"

template <>
size_t hash<int>(int val)
{
	return val;
}

int main(int argc, char **argv)
{
	HashMapLP<int, int> map;
	init(&map);
	print(&map);
	put(&map, 12, 30);
	print(&map);
	reserve(&map, 10);
	print(&map);
	put(&map, 7, 34);
	put(&map, 72, 3);
	put(&map, 44, 89);
	print(&map);
	put(&map, 72, 12);
	print(&map);
	remove(&map, 72);
	print(&map);
	reserve(&map, 20);
	print(&map);
	put(&map, 4, 10);
	print(&map);

}