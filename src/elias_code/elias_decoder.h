#ifndef ELIAS_DECODER
#define ELIAS_DECODER

#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>

using namespace std;

class Elias_decoder
{
	public:
		Elias_decoder(queue<bool> &buf);
		~Elias_decoder();
		void decode(vector<int> & output);
		int decode();
	private:	
		int ans;
		queue<bool> &buffer;
		int get_bit();

};


#endif
