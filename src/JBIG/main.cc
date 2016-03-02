#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<string>
#include<set>
#include<cassert>

#include "position.h"
#include "jbig85.h"

using namespace std;

// input functions
extern void read_pbm(string file_name, int& height, int& width, set<position>& ps);
extern void read_mtx(string file_name, int& height, int& width, set<position>& ps);

// output functions
extern void output_pbm_1(string file_stem, int height, int width, const set<position>& ps);
extern void output_pbm_4(string file_stem, int height, int width, const set<position>& ps);

// encode
extern void jbig_encode(int height, int width, set<position> &ps, string file_name, bool tp_on);

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

	//cerr << "input file: " << file_name+"."+file_ext<< endl;

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
			cerr << "File type not supported." << endl;
		}
	
	//output_pbm_1(file_name, height, width, ps);
	//output_pbm_4(file_name, height, width, ps);

	// encode image with non-progressive jbig and no typical prediction
	//jbig_encode(height, width, ps, file_name, false);
	// encode image with non-progressive jbig and typical prediction on
	jbig_encode(height, width, ps, file_name, true);

	//output_jbg(file_name, buf);
	

	return 0;
}

void output_bie(unsigned char* start, size_t len, void* file)
{
	fwrite(start, 1, len, (FILE *)file);
	return;
}

void jbig_encode(int height, int width, set<position> &ps, string file_name, bool tp_on)
{
	jbg85_enc_state se;
	file_name += ".jbg";
	FILE *fp = fopen(file_name.c_str(), "w");

	jbg85_enc_init(&se, width, height, output_bie, fp);
	if (tp_on) 
		jbg85_enc_options(&se, JBG_TPBON, 0, -1);
	else
		jbg85_enc_options(&se, 0 , 0, -1);

	set<position>::iterator it = ps.begin();

	size_t buf_size = width / 8 + 1;
	unsigned char *buf0 = new unsigned char [buf_size];
	unsigned char *buf1 = new unsigned char [buf_size];
	unsigned char *buf2 = new unsigned char [buf_size];
	unsigned char *tmp;
	int pos, offset;
	for (int i=0;i<height;++i)
	{
		int cnt = 0;
		memset(buf2, 0, buf_size);
		while (it!=ps.end() && it->row == i)
		{
			++cnt;
			pos = (it++)->col;
			offset = pos >> 3;
			buf2[offset] = buf2[offset] | (1 << (7- (pos & 7)));
		}
	//	cout << "Output line: " << i << endl;
	//	cout << cnt << endl;
		jbg85_enc_lineout(&se, buf2, buf1, buf0);
		
		tmp = buf0;
		buf0 = buf1;
		buf1 = buf2;
		buf2 = tmp;
	}
	delete [] buf0;
	delete [] buf1;
	delete [] buf2;
	fclose(fp);
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

	fout << "P4 "<< endl;
	fout << width << " " << height << endl;
	fout.close();
	fout.open(file_stem+"_check_4.pbm", ofstream::out | ofstream::app | ofstream::binary);
	unsigned char data;
	for (int i=0;i<height;++i)
	{
		for (int j=0;j<width;++j)
		{
			if (j % 8 == 0)
			{
				if (j > 0) 
					fout << data;
				data = 0;
			}

			if (ps.find(position(i,j))!=ps.end())
				data = data | (1 << (7 - j % 8));
		}
		fout << data;
	}
	fout << endl;
	//cout << data << " " << (unsigned short) data << endl;
	//cout << "tot_byte: " << cnt <<endl;

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
		{
			for (int j=0;j<width;++j)
			{
				if (j % 8 == 0) 
				{
					fin.read(buf, 1);
					//cout << *buf << endl;
				}
				int tmp = ((*buf) << (j % 8)) & (1<<7);
				if (tmp != 0) ps.insert(position(i,j));
			}
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
{
	FILE *fp;
	fp = fopen(file_name.c_str(), "r");
	if (fp == NULL)
	{
		cerr << "Error when open file" << file_name << endl;
		return ;
	}

	const int buf_size = 1 << 10;
	char* buf = new char [buf_size];

	// process MatrixMarket header
	fscanf(fp, "%*s %*s %s", buf);
	if (strcmp(buf, "coordinate") != 0)
	{
		cerr << file_name<< "is not COO format" << endl;
		return;
	}
	fgets(buf, buf_size, fp);
	
	// process comment
	char c = fgetc(fp);
	while (c == '%')
	{
		fgets(buf,buf_size,fp);
		c = fgetc(fp);
	}
	ungetc(c, fp);

	// process matrix info
	int nz;
	fscanf(fp, "%d %d %d", &height, &width, &nz);

	// process data
	ps.clear();
	int row, col;
	for (int i=0;i<nz;++i)
	{
		fscanf(fp, "%d %d", &row, &col);
		fgets(buf, buf_size, fp);
		ps.insert(position(row-1,col-1));
	}	

	fclose(fp);
}
