#include "grammar_simplifier.h"

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

	int comp = 0;
	comp += canonical[0].size() * 2 + 1;
	for (int i=1;i<count_cv;++i)
	{
		comp += canonical[i].size() * 3;
		comp -= 2;
	}
	cout << endl << endl << comp << endl << endl;
	delete [] g;
	delete [] canonical;
	delete [] used;
	delete [] appr_count;
}
