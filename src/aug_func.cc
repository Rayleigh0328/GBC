#include "position.h"

int get_diff(position p, position q, int width)
{
	return (q.row * width + q.col - p.row * width - p.col);
}

int size_byte(int n)
{
	for (int i=8;i<=64;++i)
		if (n <= (1<<i)) return i/8;
	return 4;
}
