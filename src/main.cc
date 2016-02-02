#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<string>
#include<algorithm>
#include<set>

#include "position.h"

#include "grammar_transformer.h"
#include "back_transformer.h"
#include "grammar_simplifier.h"

#define TEST 

using namespace std;

int process_argument(int argc, char** arsgv, bool* flags, string& error_msg, string& file_name);
string get_suffix(string name);
string get_prefix(string name);
void produce_coo_file(string file_name);

int main(int argc, char **argv)
{
	// process arguemnt, decide input file

	string error_msg;
	string file_name;
	bool flags[4];
	/*
	flags[0]: one_line_flag;
	flags[1]: pbm_input_flag;
	flags[2]: mtx_input_flag;
	flags[3]: code_word_level_flag;
	*/

	int ret = process_argument(argc, argv, flags, error_msg, file_name);
	if (ret != 0)
	{
		cerr << error_msg << endl;
		return ret;
	}
	string file_stem = get_prefix(file_name);

#ifdef TEST
	for (int i=0;i<4;++i)
		cout << (flags[i]?1:0);
	cout << endl;

	if (ret != 0) cout << error_msg << endl;
	else cout << file_name << endl;

	cout << "prefix:" << get_prefix(file_name) << endl;
#endif

	// process input file to get sorted COO representation
	produce_coo_file(file_name);
	
	// tranfrom COO representation to Grammar representation (One-line or regular), store in .raw file
	Grammar_transformer gt;
	gt.transform(file_stem, flags[0]);
	
	// transform raw grammar file back to COO file, called ($stem)_back.coo
	Back_transformer bt;
	bt.back_transform(file_stem + ".raw");

	// grammar simplification, store simplified grammar in .gram file
	Grammar_simplifier gs;
	gs.simplify(file_stem);
	
	bt.back_transform(file_stem + ".fin");

	// If code_word_level_flag is on, further compress Grammar representation to code word

	return 0;
}

void produce_coo_file(string file_name)
{
	ifstream fin;
	fin.open(file_name);

	int height, width;
	set<position> ps;
	ps.clear();

	int cnt;
	if (get_suffix(file_name) == "mtx")
	{
		// process mtx file

		string st;

		// skip comment
		while (fin.peek() == '%') getline(fin, st);

		//  read height, size, # of NZ
		fin >> height >> width >> cnt;

		for (int i=0;i<cnt;++i)
		{
			int row, col;
			fin >> row >> col;
			ps.insert(position(row-1, col-1));
		}
	}
	else
	{
		// process pbm file

		int tmp;
		// read image size
		fin >> width >> height;
		// process image
		for (int i=0;i<height;++i)
			for (int j=0;j<width;++j)
			{
				fin >> tmp;
				if (tmp == 1)
					ps.insert(position(i,j));
			}
	}
	fin.close();

	if (ps.size() != cnt) while (1) cout << "ERROR" << endl;
	string output_file_name = get_prefix(file_name) + ".coo";
	ofstream fout;
	fout.open(output_file_name);
	
	fout << height << " " << width << " " <<  ps.size() << endl;
	for (set<position>::iterator it = ps.begin(); it != ps.end(); ++it)
		fout << it->row << " " << it->col << endl;

	fout.close();
}

int process_argument(int argc, char **argv, bool* flags, string &error_msg, string &file_name)
{	
	/*
	L : flags[0] : one_line_flag;
	I : flags[1] : pbm_input_flag;
	M : flags[2] : mtx_input_flag;
	B : flags[3] : code_word_level_flag;
	*/
	flags[0] = false;
	flags[1] = false;
	flags[2] = true;
	flags[3] = false;
	
	for (int i=1;i<argc;++i)
		if (argv[i][0] == '-')
		{
			if (strchr(argv[i], 'L') != NULL) flags[0] = true;
			if (strchr(argv[i], 'I') != NULL) 
			{
				flags[1] = true;
				flags[2] = false;
			}
			if (strchr(argv[i], 'M') != NULL) flags[2] = true;
			if (strchr(argv[i], 'B') != NULL) flags[3] = true;
		}

	if ((flags[1] ^ flags[2]) == false)
	{
		error_msg = "Input File Format is not specified.";
		return -1;
	}

	file_name = "test.mtx";
	char tmp[1025];

	for (int i=1;i<argc;++i)
		if (argv[i][0] == '-' && strchr(argv[i], 'f') != NULL)
		{
			strcpy(tmp, argv[i+1]);
			file_name = string(tmp);
			break;
		}
	
	transform(file_name.begin(), file_name.end(), file_name.begin(), ::tolower);

	if (flags[1] && get_suffix(file_name) != "pbm")
	{
		error_msg = "File format not acceptable. Input file should have suffix pbm not " + get_suffix(file_name);
		return -1;
	}

	if (flags[2] && get_suffix(file_name) != "mtx")
	{
		error_msg = "File format not acceptable. Input file should have suffix mtx not " + get_suffix(file_name);
		return -1;
	}

	return 0;
}

string get_suffix(string name)
{
	size_t found = name.find_last_of('.');
	if (found == string::npos) return name;
	else return name.substr(found + 1);
}

string get_prefix(string name)
{
	size_t found = name.find_last_of('.');
	cout << "found: " << found << endl;
	if (found == string::npos) return name;
	else return name.substr(0, found);
}
