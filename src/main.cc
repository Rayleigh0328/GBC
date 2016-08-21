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
#include "entropy_codec.h"

//#define TEST 
//#define OUTPUT_INFO

using namespace std;

int process_argument(int argc, char** arsgv, bool* flags, string& error_msg, string& file_name);
string get_suffix(string name);
string get_prefix(string name);
void produce_coo_file(string file_name);
extern int size_byte(int);

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
	
	// transform raw grammar file back to COO file, called ($stem).raw.recon.coo
	//Back_transformer bt;
	//bt.back_transform(file_stem + ".raw");

	// grammar simplification, store simplified grammar in .gram file
	Grammar_simplifier gs;
	gs.simplify(file_stem);
	
	// transform final grammar file back to COO file, called ($stem).fin.recon.coo
	//bt.back_transform(file_stem + ".fin");

	// If code_word_level_flag is on, further compress Grammar representation to code word
	if (flags[3])
	{
		cout << "Entropy code:" << endl;
		queue<bool> bit_stream;
		while (!bit_stream.empty()) bit_stream.pop();
		Entropy_codec ec;
		ec.encode(file_stem, bit_stream);
		//ec.decode(file_stem+".bin");
	}

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
			getline(fin,st);
			ps.insert(position(row-1, col-1));
		}
	}
	else
	{
		string format;
		fin >> format >> width >> height;
		if (format == "P1")
		{
			int tmp;
			for (int i=0;i<height;++i)
				for (int j=0;j<width;++j)
				{
					fin >> tmp;
					if (tmp == 1) ps.insert(position(i,j));
				}
		}
		else if (format == "P4")
		{
			string tail;
			getline(fin, tail);

			char * buf = new char;
			for (int i=0;i<height;++i)
				for (int j=0;j<width;++j)
				{
					int num = i*width+j;
					if (num % 8 == 0) 
					{
						fin.read(buf, 1);
						//cout << *buf << endl;
					}
					int tmp = ((*buf) << (num % 8)) & (1<<7);
					if (tmp != 0) ps.insert(position(i,j));
				}
		}
		else
		{
			cout << "PBM format not supported." << endl;
		}	
	}
	fin.close();

	//if (ps.size() != cnt) while (1) cout << "ERROR" << endl;
	string output_file_name = get_prefix(file_name) + ".coo";
	ofstream fout;
	fout.open(output_file_name);
	
#ifdef OUTPUT_INFO
	cout << height << " " << width << " " <<  ps.size() << endl;
	cout << "CRS :" << ps.size() * size_byte(width) + height * size_byte(ps.size()) << endl << endl;
#endif

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
	if (found == string::npos) return name;
	else return name.substr(0, found);
}
