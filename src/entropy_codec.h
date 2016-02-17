#ifndef ENTROPY_CODEC_H
#define ENTROPY_CODEC_H

#include<set>
#include<queue>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdio>

#include "position.h"
#include "./arithmetic_code/ac_encoder.h"

class Entropy_codec{
public:
	Entropy_codec(){};
	~Entropy_codec(){};

	void encode(string file_stem, queue<bool>& output);
	//void decode();
private:
	int height, width, tot_var;
	void output_pbm_4(string file_stem, int height, int width, const set<position>& ps);
	void encode_label(AC_encoder &enc, int label, queue<bool>& output);
};

#endif
