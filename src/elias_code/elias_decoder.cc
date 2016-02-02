#include "elias_decoder.h"

Elias_decoder::Elias_decoder(queue<bool> &buf):
	buffer(buf)
{
	ans = 1;
}

Elias_decoder::~Elias_decoder(){};

int Elias_decoder::get_bit()
{
	if (buffer.empty()) return 0;
	int ans = (int) buffer.front();
	buffer.pop();
	return ans;
}

void Elias_decoder::decode(vector<int> & output)
{
	int tmp;
	ans = 1;
	while ((tmp = get_bit()) != 0)
	{
		for (int i=0;i<ans;++i)
			tmp = (tmp << 1) ^ get_bit();
		ans = tmp;
	}
	//printf("%d\n",ans);
	output.push_back(ans);
}

int Elias_decoder::decode()
{
	int tmp;
	ans = 1;
	while ((tmp = get_bit()) != 0)
	{
		for (int i=0;i<ans;++i)
			tmp = (tmp << 1) ^ get_bit();
		ans = tmp;
	}
	return ans;
}
