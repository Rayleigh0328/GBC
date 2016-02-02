#ifndef BACK_TRANSFORMER_H
#define BACK_TRANSFORMER_H

#include<cstring>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<cstdlib>
#include<set>
#include<map>

#include "position.h"

using namespace std;

class Back_transformer
{
public:
	Back_transformer();
	void back_transform(string file_name);

private:
	string file_stem;
	set<position>* pattern;

	int height, width;
	
	int tot_var;
	map<position, int> *cg;
	typedef map<position, int>::iterator cg_map_it;

	//void decode_sizes(queue<bool>& buf);
	//void decode_residual_structure(queue<bool>& buf);
	//void decode_var_structure(queue<bool>& buf);
	//void decode_label(queue<bool>& buf);
	//void decode_label_elias(queue<bool>& buf);
	void reconstruct_image();

	void create_image();
	void render_variables();
	void render(int pos, bool* flag);

	// Methods for testing 
    //void output_pattern(int pattern_number);
	//void output_pattern_image(int id, int p);
};

#endif
