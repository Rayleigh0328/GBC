#ifndef ELIAS_ENCODER
#define ELIAS_ENCODER

#include<cstdio>
#include<iostream>
#include<queue>

using namespace std;

class Elias_encoder
{
public:
		Elias_encoder();
		~Elias_encoder();

		void refresh();
		int encode(int k,queue<bool>& output);
private:
		static const int maxsize = 100;
		int *stack;
		int top;

		void convert_to_binary(int k);
};



#endif
