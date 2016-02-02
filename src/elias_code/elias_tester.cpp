#include<iostream>
#include<fstream>
#include<cstdio>
#include<ctime>
#include<cstdlib>

#include "elias_encoder.h"
#include "elias_decoder.h"

using namespace std;

int main()
{
	ifstream fin;

	int n,bound;
	
	fin.open("elias_test_configuration");
	fin>>n>>bound;
	fin.close();

	printf("number of numbers: %d\n", n);
	printf("upper bound of each number: %d\n", bound);

	Elias_encoder enc;

	srand(time(NULL));
	
	ofstream fout;
	fout.open("elias_input");

	queue<bool> output;
	for (int i=0;i<n;++i)
	{
		int tmp =  rand() % bound + 1;
		fout<< tmp << " ";
		enc.encode(tmp, output);
	}
	fout<<endl;
	fout.close();

	fout.open("elias_output");

	Elias_decoder dec(output);
	while (!output.empty())
	{
		int tmp = dec.decode();
		fout<< tmp <<" ";
	}
	fout<<endl;
	fout.close();

	return 0;
}
