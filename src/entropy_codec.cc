#include "entropy_codec.h"

void Entropy_codec::encode(string file_stem, queue<bool>& output)
{
	string input_file_name = file_stem + ".fin";
	string output_file_name = file_stem + ".bin";

	ifstream fin;
	fin.open(input_file_name);
	
	// read input file

	fin >> height >> width >> tot_var;

	queue<bool> num_buf;
	while (!num_buf.empty()) num_buf.pop();

	//encode_nums(num_buf);

	++tot_var;

	string line;
	getline(fin, line);
	
	//process first line
	getline(fin, line);
	stringstream ss(line);

	set<position> ps;
	ps.clear();

	queue<bool> label_buf;
	while (!label_buf.empty()) label_buf.pop();

	queue<bool> struct_buf;
	while (!struct_buf.empty()) struct_buf.pop();

	int row, col, label;

	AC_encoder enc;

	while (ss >> row >> col >> label)
	{
		ps.insert(position(row, col));
		encode_label(enc, label,label_buf);
	}

	output_pbm_4(file_stem, height, width, ps);

	for (int i=1;i<tot_var;++i)
	{	
		getline(fin, line);
		stringstream ss(line);

		ss >> label;
		encode_label(enc, label, label_buf);

		while (ss >> row >> col >> label)
		{
			//encode_structure(row, col);
			encode_label(enc, label, label_buf);
		}
	}
	
	enc.encode_symbol(0, label_buf);
	cout <<"label buf size:" <<  label_buf.size() << endl;
	//output.clear();
}


void Entropy_codec::encode_label(AC_encoder& enc, int label, queue<bool>& output)
{
	enc.encode_symbol(label + 1, output);	
}

void Entropy_codec::output_pbm_4(string file_stem, int height, int width, const set<position>& ps)
{   
	ofstream fout;
	fout.open(file_stem+"_v0.pbm");

	fout << "P4 "<< width << " " << height << endl;
	fout.close();
	fout.open(file_stem+"_v0.pbm", ofstream::out | ofstream::app | ofstream::binary);
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
	fout << data << endl;

	fout.close();
}

