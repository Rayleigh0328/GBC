#ifndef HUFFMAN_CODEC
#define HUFFMAN_CODEC

#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
#include <stack>

using namespace std;

class huffman_codec
{
public:
	// public methods
	// no public member
	huffman_codec(int alphabet_size = 256);
	~huffman_codec();
	void encode(int x, queue<bool>& output);
	void decode(queue<bool>& buffer, vector<int>& output);
	int decode(queue<bool>& buffer);

private:
	// private members
	int n, m, e, r, z;
	int* alpha;
	int* rep;
	int* block;
	int* weight;
	int* parent;
	int* parity;
	int* rt_child;
	int* first;
	int* last;
	int* prev_block;
	int* next_block;
	int avail_block;
	stack<bool> encode_stack;

	// private methods
	void encode_and_transmit(int x, queue<bool>& output);
	int receive_and_decode(queue<bool>& buf);

	void update(int k);
	int find_child(int j, int parity);
	void interchange_leaves(int e1, int e2);
	void slide_and_increment(
				int &q, int &leaf_to_increment, int &bq, int &b, 
				int &old_parent, int &old_parity, int &nbq, int &par, int &bpar, bool &slide);
};


#endif




