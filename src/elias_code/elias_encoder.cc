#include "elias_encoder.h"

Elias_encoder::Elias_encoder()
{
	stack = new int [maxsize];
	refresh();
}

Elias_encoder::~Elias_encoder()
{
	delete [] stack;
}

void Elias_encoder::refresh()
{
	stack[0] = 0;
	top = 1;
}

int Elias_encoder::encode(int k, queue<bool>& output)
{
	int tmp = k;
	int old_top = top;
	while (tmp != 1)
	{
		convert_to_binary(tmp);
		tmp = top - old_top - 1;
		old_top = top;
	}
	for (int i=top-1;i>=0;--i)
		output.push((bool)stack[i]);
		//printf("%d\n", stack[i]);
	refresh();
}

void Elias_encoder::convert_to_binary(int k)
{
	while (k!=0)
	{
		stack[top++] = k & 1;
		k = k >> 1;
	}
}
