#ifndef GRAMMAR_TRANSFORMER_H 
#define GRAMMAR_TRANSFORMER_H

#include<cstdio>
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<utility>
#include<ctime>
#include<algorithm>
#include<unordered_map>
#include<set>
#include<queue>
#include<vector>
#include<map>

#include "position.h"
#include "interval.h"
#include "variable.h"
#include "grammar_transformer.h"

using namespace std;

class Grammar_transformer{
public:
	Grammar_transformer();
	void transform(string stem, bool mode_flag, int round_limit = 7);

private:
	// private members
	string file_stem;
	bool one_line_flag;
	int round_dist_limit;
	int round_number = 10;

	int height, width;
	int var_counter;

	set<position> residual;
	unordered_map<position, int, position_hasher> label_map;

	multiset<interval> interval_set;

	unordered_map<variable, int, variable_hasher> var_map;
	unordered_map<variable, pair<position, position> , variable_hasher> var_cand_map;

	typedef set<position>::iterator residual_it;
	typedef unordered_map<variable, pair<position, position>, variable_hasher>::iterator var_cand_map_it;
	typedef unordered_map<variable, int, variable_hasher>::iterator var_map_it;
	typedef map<position, int>::iterator cg_map_it;

	// private methods
	void init_residual();
	void scan_matrix(int round);
	void process_pair(position p1, position p2);
	void remove_position(position p);
	void output_raw_grammar();

	//void get_canonical_form();

	// methods for testing 
	/*
	void output_residual(const char* file_name);
	void print_typical_time();
	void output_double_check_file();
	void run_length_encode();
	void output_variable_info();
	void output_grammar_info();
	*/
};

#endif
