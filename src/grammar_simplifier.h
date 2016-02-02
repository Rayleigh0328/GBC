#ifndef GRAMMAR_SIMPLIFIER_H
#define GRAMMAR_SIMPLIFIER_H

#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<string>
#include<cstring>
#include<map>
#include<queue>

#include "position.h"

using namespace std;

class Grammar_simplifier
{
public:
	Grammar_simplifier();
	void simplify(string file_stem);
private:
	int height, width, tot_var;
};

#endif
