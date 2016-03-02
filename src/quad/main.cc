/*

Quad-tree method, call
	quad -f file_name -t tile_size
	default file_name is test.mtx
	default tile_size is 256
*/

#include<iostream>
#include<cstdio>
#include<fstream>
#include<sstream>
#include<string>
#include<cstring>

#include "quad_tree_transformer.h"

using namespace std;

void process_argument(int argc, char **argv, string& file_name, int& tile_size);

int main(int argc, char **argv)
{
	string file_name;
	int tile_size;

	process_argument(argc, argv, file_name, tile_size);

	Quad_tree_transformer qt;
	double size = qt.transform(file_name, tile_size);
	cout << file_name << " " << size << endl;

	return 0;
}

void process_argument(int argc, char **argv, string& file_name, int& tile_size)
{
	file_name = "test.mtx";
	for (int i=1;i<argc;++i)
		if (argv[i][0]=='-' && argv[i][1] == 'f')
		{
			file_name = string(argv[i+1]);
			break;
		}
	
	tile_size = 256;
	for (int i=1;i<argc;++i)
		if (argv[i][0]=='-' && argv[i][1] == 't')
		{
			tile_size = atoi(argv[i+1]);
			break;
		}
}
