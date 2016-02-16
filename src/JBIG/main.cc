#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<string>
#include<set>
#include<cassert>

#include "position.h"

using namespace std;

extern void read_pbm(string file_name, int& height, int& width, set<position>& ps);
extern void read_mtx(string file_name, int& height, int& width, set<position>& ps);
extern void output_pbm_1(string file_stem, int height, int width, const set<position>& ps);
extern void output_pbm_4(string file_stem, int height, int width, const set<position>& ps);

int main(int argc, char** argv)
{
	string file_full;
	string file_name;
	string file_ext;

	if (argc < 2) 
	{
		file_full = "test.pbm";
		file_name = "test";
		file_ext = "pbm";
	}
	else 
	{
		file_full = string(argv[1]);
		char* pos = strrchr(argv[1], '.');
		*pos = '\0';
		file_name = string(argv[1]);
		file_ext = string(pos+1);
	}

	cout << "input file: " << file_name+"."+file_ext<< endl;

	set<position> ps;
	ps.clear();
	int height, width;

	if (file_ext == "pbm")
	{
		read_pbm(file_full, height, width, ps);
	}
	else
		if (file_ext == "mtx")
		{
			read_mtx(file_full, height, width, ps);
		}
		else
		{
			cout << "File type not supported." << endl;
		}
	
	//output_pbm_1(file_name, height, width, ps);
	//output_pbm_4(file_name, height, width, ps);

	JBIG_encoder enc;
	queue<bool> buf;
	enc.encode(height, width, ps, buf);

	output_jbg(file_name, buf);
	

	return 0;
}

void output_pbm_1(string file_stem, int height, int width, const set<position> &ps)
{
	ofstream fout;
	fout.open(file_stem+"_check_1.pbm");
	fout << "P1" << " " << width << " " << height << endl;
	
	for (int i=0;i<height;++i)
	{
		for (int j=0;j<width;++j)
			if ( ps.find(position(i,j))!=ps.end()) 
				fout << "1 ";
			else 
				fout << "0 ";
		fout << endl;
	}
	fout.close();
}

void output_pbm_4(string file_stem, int height, int width, const set<position> & ps)
{
	ofstream fout;
	fout.open(file_stem+"_check_4.pbm");

	fout << "P4 "<< width << " " << height << endl;
	fout.close();
	fout.open(file_stem+"_check_4.pbm", ofstream::out | ofstream::app | ofstream::binary);
	char data;
	int cnt = 0;
	for (int i=0;i<height;++i)
		for (int j=0;j<width;++j)
		{
			int num = i*width + j;
			if (num % 8 == 0)
			{
				if (num > 0) 
				{
					fout << data;
					++cnt;
				}
				data = 0;
			}

			if (ps.find(position(i,j))!=ps.end())
				data += 1 << (7 - num % 8);
		}
	//cout << data << " " << (unsigned short) data << endl;
	//cout << "tot_byte: " << cnt <<endl;
	fout << data << endl;

	fout.close();
}

void read_pbm(string file_name, int& height, int& width, set<position>& ps)
{
	ifstream fin;
	fin.open(file_name);
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

		//fin.read(buf,1);
		//printf("%d\n", *buf);
		//fin.read(buf,1);
		//printf("%d\n", *buf);
		//assert(fin.eof());
		//cout << "cnt read: " << cnt << endl;
	}
	else
	{
		cout << "PBM format not supported." << endl;
	}
	fin.close();
}

void read_mtx(string file_name, int& height, int& width, set<position>& ps)
{}
