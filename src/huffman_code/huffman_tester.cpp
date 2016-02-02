#include <iostream>
#include <fstream>
#include <cstdio>
#include <queue>
#include <stack>
#include <set>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "huffman_codec.h"

//#define TEST_HUF

using namespace std;

void print_buffer(queue<bool> copy_buf)
{
	cout<<"buffer size = "<< copy_buf.size()<<endl;
	while (!copy_buf.empty())
	{
		cout<< (copy_buf.front()?1:0);
		copy_buf.pop();
	}
	cout<<endl;
}

int main()
{
	ifstream fin;
	fin.open("test_configuration");

	int alphabet_size, length;
	fin >> alphabet_size >> length;
	
	printf("test alphabet size is: %d\n", alphabet_size);
	printf("test sequence length is %d\n", length);

	fin.close();


	huffman_codec enc(alphabet_size);
	huffman_codec dec(alphabet_size);

	srand(time(NULL));
	
	vector<int> msg;
	for (int i=0;i<length;++i)
		msg.push_back( rand() % alphabet_size + 1);

#ifdef TEST_HUF
	for (int i=0;i<length;++i)
		cout<< msg[i] << " ";
	cout<<endl;
#endif
	
	queue<bool> buf;
	for (int i=0;i<length;++i)
		enc.encode(msg[i], buf);
	
#ifdef TEST_HUF
	cout<< "Finish Encoding"<<endl;
	print_buffer(buf);
#endif


	vector<int> result;
	dec.decode(buf, result);

#ifdef TEST_HUF
	for (int i=0;i<length;++i)
		cout<< result[i] << " ";
	cout<<endl;
#endif

	bool check_same = true;
	for (int i=0;i<length;++i)
		if (msg[i] != result[i])
		{	
			check_same = false;
			break;
		}
	
	if (check_same) cout<< "Correct" << endl;
	else cout<<"Wrong"<<endl;
	return 0;
}
