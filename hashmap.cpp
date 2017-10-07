#include <cstdio>

#include "hashmap.hpp"

template <>
size_t hash<int>(int val)
{
	return 0;
}

int main(int argc, char **argv)
{
	HashMap<int, int> map;
	if (init(&map, 10))
		abort();
	
	for (int i = 0;; ++i)
	{
		if (put(&map, i, 10 - i))
			break;
		printf("put [%d, %d] to map \n", i, 10 - i);
	}
	printf("map exhausted \n");
	
	printf("removing [5, 5] from map\n");
	remove(&map, 5);
	if (get(&map, 5))
		abort();
	printf("successfully removed!\n");
	
	if (put(&map, 100, 25))
		abort();
	printf("put [100, 25] in map\n");
	
	close(&map);
}