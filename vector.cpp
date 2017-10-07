#include "vector.hpp"

int main(int argc, char **argv)
{
	Vector<int> vec;
	init(&vec);
	if(resize(&vec, 10))
		abort();
	for (auto iter = begin(&vec); iter < end(&vec); ++iter)
	{
		*iter = (int) (__intptr_t) iter;
	}
	for (auto iter : &vec)
	{
		printf("%d\n", iter);
	}
	close(&vec);
}
