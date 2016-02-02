#include "back_transformer.h"

//#define OUTPUT_PATTERN

Back_transformer::Back_transformer(){}

void Back_transformer::render(int v_pos, bool *flag)
{
	flag[v_pos] = true;

	for (cg_map_it it = cg[v_pos].begin(); it != cg[v_pos].end(); ++it)
	{
		if (it->second > 0 && !flag[it->second]) render(it->second, flag);
	}

	set<position> &result = pattern[v_pos];
	set<position>::iterator pit;

	for (cg_map_it it = cg[v_pos].begin(); it != cg[v_pos].end(); ++it)
	{
		if (it->second == 0) result.insert(position(it->first.row, it->first.col));
		else 
		{
			set<position>& s = pattern[it->second];
			for (pit = s.begin(); pit != s.end(); ++pit)
				result.insert(position(pit->row + it->first.row, pit->col + it->first.col));
		}
	}
}

void Back_transformer::render_variables()
{
	pattern = new set<position> [tot_var];
	for (int i=0;i<tot_var;++i) pattern[i].clear();

	bool *flag = new bool [tot_var];
	for (int i=0;i<tot_var;++i) flag[i] = false;

	for (int i=0;i<tot_var;++i)
		if (!flag[i]) 
			render(i, flag);

	delete [] flag;
}

void Back_transformer::reconstruct_image()
{
	render_variables();

	#ifdef OUTPUT_PATTERN
		//output_pattern(10);
		//output_largest_pattern(10);
	#endif

	set<position>& shape = pattern[0];
	set<position> result;
	result.clear();

	for (set<position>::iterator pit = shape.begin(); pit != shape.end(); ++pit)
		result.insert(position(height - pit->row, width - pit->col));

	ofstream fout;
	fout.open(file_stem + ".recon.coo");

	fout << height << " " << width << " " << result.size() << endl;
	for (set<position>::iterator it = result.begin(); it != result.end(); ++it)
		fout<< it->row << " " << it->col << endl;

	fout.close();
	delete [] pattern;
}

/*
void Back_transformer::create_image()
{
	image = new bool* [height];
	for (int i=0;i<height;++i)
		image[i] = new bool [width];
	for (int i=0;i<height;++i)
		for (int j=0;j<width;++j)
			image[i][j] = false;
}
*/

void Back_transformer::back_transform(string file_name)
{
	file_stem = file_name;

	ifstream fin;
	fin.open(file_name);
	
	fin >> height >> width >> tot_var;
	++tot_var;

	cg = new map<position, int> [tot_var];
	for (int i=0;i<tot_var;++i) cg[i].clear();
	
	string line;
	getline(fin, line);
	getline(fin, line);

	stringstream ss(line);

	int row, col, label;
	while (ss >> row >> col >> label)
		cg[0].insert(pair<position, int>(position(row,col), label));

	for (int i=1;i<tot_var;++i)
	{	
		getline(fin, line);
		stringstream ss(line);

		ss >> label;
		cg[i].insert(pair<position, int>(position(0, 0), label));

		while (ss >> row >> col >> label)
			cg[i].insert(pair<position, int>(position(row, col), label));
	}
	
	reconstruct_image();

	delete [] cg;
}
/*
void Back_transformer::output_pattern(int pattern_number)
{
	int *a = new int [tot_var];
	for (int i=0;i<tot_var;++i) a[i] = i;

	int tmp;
	for (int i=0;i<tot_var;++i)
		for (int j=i+1;j<tot_var;++j)
			if (pattern[a[i]].size() < pattern[a[j]].size())
			{
				tmp = a[i];
				a[i] = a[j];
				a[j] = tmp;
			}
	
	cout << "Pattern sizes: "<<endl<<"\t";
	for (int i=0;i<tot_var;++i)
		cout << pattern[a[i]].size() << " ";
	cout << endl;

	for (int i=0;i<pattern_number;++i)
		output_pattern_image(i,a[i]);


	int *cnt_appr = new int [tot_var];
	for (int i=0;i<tot_var;++i)
	{
		cnt_appr[i] = 0;
		for (set<position>::iterator it = residual.begin(); it != residual.end(); ++it)
			if (label_map[*it] == a[i]) ++cnt_appr[i];
	}

	cout << "Pattern apperance: " << endl << "\t";
	for (int i=0;i<tot_var;++i)
		printf("\t%dth largest pattern appear %d times.\n", i, cnt_appr[i]);
}

void Back_transformer::output_pattern_image(int id, int p)
{
	set<position>& pat = pattern[p];
	cout << "Pattern #" << id << ":" << endl;
	//cout << "original pattern: ";
	//for (set<position>::iterator it = pat.begin(); it != pat.end(); ++it)
	//{
	//	cout << "(" << it->row << "," << it->col << ")  " ;
	//}
	//cout << endl;

	int min_row = (1<<30);
	int min_col = (1<<30);
	int max_row = 0;
	int max_col = 0;
	for (set<position>::iterator it = pat.begin(); it != pat.end(); ++it)
	{
		min_row = min(min_row, it->row);
		min_col = min(min_col, it->col);
	}

	//cout << "OUTPUT PATTERN IMAGE" << id << endl;
	//cout << "\tmin_row: "<< min_row << endl;
	//cout << "\tmin_col: "<< min_col << endl;

	set<position> shifted;
	shifted.clear();
	for (set<position>::iterator it = pat.begin(); it != pat.end(); ++it)
	{
		shifted.insert(position(it->row - min_row, it->col - min_col));
	}
	//cout << "\t\n Get shifted pattern" << endl;
	for (set<position>::iterator it = shifted.begin(); it != shifted.end(); ++it)
	{
		max_row = max(max_row, it->row);
		max_col = max(max_col, it->col);
	}
	++max_row;
	++max_col;

	cout << "\tmax_row: "<< max_row << endl;
	cout << "\tmax_col: "<< max_col << endl;

	bool **pi = new bool* [max_row];
	for (int i=0;i<max_row;++i)
		pi[i] = new bool [max_col];

	for (int i=0;i<max_row;++i)
		for (int j=0;j<max_col;++j)
			pi[i][j] = false;

	//cout << "\timage created" << endl;

	//cout << "\t shifted image:"<<endl;
	//for (set<position>::iterator it = shifted.begin(); it != shifted.end(); ++it)
	//	cout << "(" << it->row << "," << it->col << ")" <<endl ;

	for (set<position>::iterator it = shifted.begin(); it != shifted.end(); ++it)
		pi[it->row][it->col] = true;
	

	cout << "\timage written" << endl;

	char filename[] = "pattern00.pbm";
	filename[7] += id / 10;
	filename[8] += id % 10;

	ofstream fout;
	fout.open(filename);

	fout << "P1" << endl;
	fout << max_col << " " << max_row << endl;

	for (int i=0;i<max_row;++i)
	{
		for (int j=0;j<max_col;++j)
			fout << (pi[i][j]?1:0) << " ";
		fout << endl;
	}

	for (int i=0;i<max_row;++i)
		delete pi[i];
	delete pi;

	fout.close();
}
*/


