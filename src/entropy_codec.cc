#include "entropy_codec.h"

void Entropy_codec::encode(string file_stem, queue<bool>& output)
{
	string input_file_name = file_stem + ".fin";
	string output_file_name = file_stem + ".bin";

	ifstream fin;
	fin.open(input_file_name);
	
	// read input file
	fin >> height >> width >> tot_var;
	++tot_var;

	
	// encode matrix size and number of variables
	Elias_encoder e_enc;
	e_enc.encode(height, output);
	e_enc.encode(width, output);
	e_enc.encode(tot_var, output);

	cout << "Buffer size after encode numbers: " << output.size() << endl;

	string line;
	getline(fin, line);
	
	//process first line
	getline(fin, line);
	stringstream ss(line);


	int row, col, label;

	AC_encoder enc;

	queue<bool> label_buf;
	while (!label_buf.empty()) label_buf.pop();

	int prev = -1;
	while (ss >> row >> col >> label)
	{
		int cur = row * width + col;
		e_enc.encode(cur-prev, output);
		prev = cur;

		encode_label(enc, label,label_buf);
	}
	// terminate encoding of v0 structure by encode ()
	e_enc.encode(width*height-prev,output);

	cout << "Buffer size after encode structure of V0: " << output.size() << endl;

	//output_pbm_4(file_stem, height, width, ps);

	for (int i=1;i<tot_var-1;++i)
	{	
		getline(fin, line);
		stringstream ss(line);

		ss >> label;
		encode_label(enc, label, label_buf);

		vector<position> pv;
		pv.clear();
		while (ss >> row >> col >> label)
		{
			//encode_structure(row, col);
			pv.push_back(position(row, col));
			encode_label(enc, label, label_buf);
		}
		int min_col = -1;
		for (int i=0;i<pv.size();++i)
			min_col = min(min_col, pv[i].col);

		if (pv.size() == 0) cerr << "variable " << i << endl;
		//assert(pv.size() > 0);
		e_enc.encode(pv.size(), output);
		for (int i=0;i<pv.size();++i)
		{
			e_enc.encode(pv[i].row+1, output);
			e_enc.encode(pv[i].col-min_col+1, output);
		}
	}

	cout << "Buffer size after encode structure of other variables: " << output.size() << endl;
	// terminate lable encoding by pass 0 to ac	
	enc.encode_symbol(0, label_buf);

	cout <<"label buf size:" <<  label_buf.size() << endl;

	int result = output.size() + label_buf.size();
	cout << "tot size: " << result << endl;
	cout << "tot byte: " << result /8.0 << endl;
	cout << "tot kb  : " << result / 8.0 / 1024 << endl;
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

