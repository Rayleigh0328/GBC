#ifndef QUAD_TREE_TRANSFORMER_H
#define QUAD_TREE_TRANSFORMER_H

#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <set>
#include <cmath>

#include "position.h"

using namespace std;

class Quad_tree_transformer{
public:
	Quad_tree_transformer();
	double transform(string file_name, int tile_size);
private:
	
	struct node
	{
		int height, width;
		set<position> ps;
		node* child[4];
	};

	typedef set<position>::iterator pit;
	typedef set<position>::const_iterator cpit;

	void split(node* p, int tile_size);
	void process_point(node* p, int c_num, int row, int col);
	int get_complexity(node* p, int tile_size);
	void transform_back(node* root, int tile_size, ofstream &fout);
	void construct_ps(node* p, int tile_size, set<position>& ans, int hb, int wb);
	int get_byte(int t);
};

#endif
