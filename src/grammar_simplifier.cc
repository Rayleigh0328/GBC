#include "grammar_simplifier.h"

#define OUTPUT_STAT

extern int size_byte(int n);
extern int get_diff(position p, position q, int width);

Grammar_simplifier::Grammar_simplifier(){}

void Grammar_simplifier::simplify(string file_stem)
{
	// read input file store raw grammar into g
	string file_name = file_stem + ".raw";
	ifstream fin;
	fin.open(file_name);

	fin >> height >> width >> tot_var;
	++tot_var;

	typedef map<position, int>::iterator g_map_it;
	map<position, int> *g = new map<position,int> [tot_var];
	for (int i=0;i<tot_var;++i) g[i].clear();

	string line;
	getline(fin, line);
	getline(fin, line);

	stringstream s(line);
	int r, c, l;
	while (s >> r >> c >> l)
		g[0].emplace(position(r, c), l);
	
	for (int i=1;i<tot_var;++i)
	{
		getline(fin, line);
		stringstream ss(line);
		
		ss >> l;
		g[i].emplace(position(0,0),l);

		while (ss >> r >> c >> l)
			g[i].emplace(position(r,c),l);
	}

	fin.close();

	// count appearance of each variable
	int *appr_count = new int [tot_var];
	for (int i=0;i<tot_var;++i) appr_count[i] = 0;

	for (int i=0;i<tot_var;++i)
		for (g_map_it it = g[i].begin(); it != g[i].end(); ++it)
			++appr_count[it->second];

	// process each variable, replace useless variable and reorder
	queue<int> var_que;
	while (!var_que.empty()) var_que.pop();

	map<int, int> reorder_map;
	reorder_map.clear();

	bool *used = new bool [tot_var];
	for (int i=0;i<tot_var;++i) used[i] = false;

	map<position, int> *canonical = new map<position,int> [tot_var];
	for (int i=0;i<tot_var;++i) canonical[i].clear();

	var_que.push(0);
	used[0] = true;
	int count_cv = 0;

	while (!var_que.empty())
	{
		int label = var_que.front();
		var_que.pop();
		reorder_map.emplace(label, count_cv);

		for (g_map_it it = g[label].begin(); it != g[label].end(); ++it)
			canonical[count_cv].insert(*it);

		bool all_useful = false;
		while (!all_useful)
		{
			all_useful = true;
			for (g_map_it it = canonical[count_cv].begin(); it != canonical[count_cv].end(); )
				if (appr_count[it->second] == 1)
				{
					all_useful = false;
					int target_label = it->second;
					int ox = it->first.row;
					int oy = it->first.col;
			
					map<position,int>::const_iterator tmp_it = it++;
					canonical[count_cv].erase(tmp_it);

					for (g_map_it sub_it = g[target_label].begin(); sub_it != g[target_label].end(); ++sub_it)
					{
						int dx = sub_it->first.row;
						int dy = sub_it->first.col;
						canonical[count_cv].emplace(position(ox+dx, oy+dy), sub_it->second);
					}
				}
				else
				{
					++it;
				}
		}

		for (g_map_it it = canonical[count_cv].begin(); it != canonical[count_cv].end(); ++it)
		{
			if (!used[it->second]) 
			{
				var_que.push(it->second);
				used[it->second] = true;
			}
		}

		++count_cv;
	}
	
	// change all labels to new ones
	for (int i=0;i<tot_var;++i)
		for (g_map_it it = canonical[i].begin(); it != canonical[i].end(); ++it)
			it->second = reorder_map[it->second];

	// output final grammar
	ofstream fout;
	file_name = file_stem + ".fin";
	fout.open(file_name);

	fout << height << " " << width << " " << count_cv << endl;
	for (g_map_it it = canonical[0].begin(); it != canonical[0].end(); ++it)
		fout << it->first.row << " " << it->first.col << " " << it->second << " ";
	fout << endl;

	for (int i=1;i<count_cv;++i)
	{
		for (g_map_it it = canonical[i].begin(); it != canonical[i].end(); ++it)
			if (it == canonical[i].begin())
				fout << canonical[i].begin()->second << " ";
			else
				fout << it->first.row << " " << it->first.col << " " << it->second << " ";
		fout << endl;
	}
	
	fout.close();
#ifdef OUTPUT_STAT

	cout << "|V|: " << count_cv << endl;
	cout << "V0: " << canonical[0].size() << endl;
	int tot_edge = 0;
	for (int i=0;i<count_cv;++i) 
		tot_edge += canonical[i].size();
	cout << "Total edge: " << tot_edge << endl;

	int cnt_term = 0;
	for (g_map_it it=canonical[0].begin(); it != canonical[0].end(); ++it)
		if (it->second == 0) 
			++cnt_term;
	cout << "cnt_term:" << cnt_term << endl;
	int cnt_nonterm = canonical[0].size() - cnt_term;

	int comp = 4;

	int term_coo = 2 * cnt_term;
	int term_csr = cnt_term + height;
	if (term_coo <= term_csr)
	{
		comp += term_coo;
		cout << "V0 terminal edge representation: COO" << endl;
	}
	else
	{
		comp += term_csr;
		cout << "V0 terminal edge representation: CSR" << endl;
	}

	int nonterm_coo = 3 * cnt_nonterm;
	int nonterm_csr = 2 * cnt_nonterm + height;
	if (nonterm_coo <= nonterm_csr)
	{
		comp += nonterm_coo;
		cout << "V0 non-t edge representation: COO" << endl;
	}
	else
	{
		comp += nonterm_csr;
		cout << "V0 non-t edge representation: CSR" << endl;
	}
	/*
	int coo_size = canonical[0].size() * 3;
	int csr_size = canonical[0].size() * 2 + height;

	int comp = 3;
	if (coo_size <= csr_size)
	{
		comp += coo_size;
		cout << "V0 representation: COO" << endl;
	}
	else
	{
		comp += csr_size;
		cout << "V0 representation: CSR" << endl;
	}
	*/
	int count_two = 0;
	for (int i=1;i<count_cv;++i)
	{
		if (canonical[i].size()==2) 
		{
			++count_two;
			comp += 4;
		}
		else
		{
			comp += 1;
			comp += (canonical[i].size()-1) * 3 + 1;
		}
		//comp -= 2;
	}
	cout << endl << endl << comp << endl;
	
	cout << "Variable of size 2: " << count_two << endl<<endl<<endl<<endl;


	/*
	cout << "|V|: " << count_cv << endl;
	cout << "V0: " << canonical[0].size() << endl;
	int tot_edge = 0;
	for (int i=0;i<count_cv;++i) 
		tot_edge += canonical[i].size();
	cout << "Total edge: " << tot_edge << endl;
	
	int max_diff = 0;
	position prev(0,0);
	for (g_map_it it = canonical[0].begin(); it!= canonical[0].end(); ++it)
	{	
		max_diff = max(max_diff, get_diff(prev, it->first, width));
		prev = it->first;
	}

	int comp =  (size_byte(max_diff) + size_byte(count_cv)) * canonical[0].size();
	cout << "Max run length in V0: " << max_diff << endl;
	cout << "Memory for V0: " <<  comp << endl;

	int count_two = 0;
	for (int i=1;i<count_cv;++i)
	{
		if (canonical[i].size()==2) ++count_two;

		comp += canonical[i].size() * size_byte(count_cv);
		comp += (canonical[i].size()-1) * 2;
		//comp -= 2;
	}
	cout << endl << endl << comp << endl << endl;
	
	cout << "Variable of size 2: " << count_two << endl;
	*/
#endif

	delete [] g;
	delete [] canonical;
	delete [] used;
	delete [] appr_count;
}
