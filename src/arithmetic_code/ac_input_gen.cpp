#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <cstdlib>

#define INPUT_LENGTH 10000

using namespace std;

int main()
{
	srand (time(NULL));
	ofstream fout;
	fout.open("ac_input");
	
	int range = 1;
	for (int i=0;i<INPUT_LENGTH;++i)
	{
		int tmp = rand() % range + 1;
		if (tmp == range) ++range;
		fout<<tmp<<endl;
	}

	fout << 0 << endl;
	fout.close();
	return 0;
}
