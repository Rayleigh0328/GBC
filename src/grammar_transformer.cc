#include "grammar_transformer.h"

Grammar_transformer::Grammar_transformer(){}

void Grammar_transformer::transform(string stem, bool mode_flag, int round_limit)
{
	// record arguments
	file_stem = stem;
	one_line_flag = mode_flag;
	round_dist_limit = round_limit;

	// read input file
	init_residual();

	// Initialize data structure
	var_counter = 0;
	var_map.clear();
	var_cand_map.clear();

	// scan image
	for (int round=0;round<round_number;++round)
	{
		scan_matrix(round);
	}

	// output raw grammar
	output_raw_grammar();
}

void Grammar_transformer::scan_matrix(int round)
{
	int dist_limit = min((1<<round),(1<<round_dist_limit));
	//if (dist_limit == (1<<MAX_DIST_ROUND)) --dist_limit;

	residual_it pit = residual.begin();
	position cur_pos= *pit;

	interval_set.clear();
	interval_set.insert(interval(cur_pos));
	int scan_line = cur_pos.row;

	var_cand_map.clear();

	for ( ++pit; pit != residual.end(); ++pit)
	{
		cur_pos = *pit;

		scan_line = cur_pos.row;

		multiset<interval>::iterator low_it  = interval_set.lower_bound(interval(position(0, cur_pos.col - scan_line - dist_limit)));
		multiset<interval>::iterator high_it = interval_set.upper_bound(interval(position(0, cur_pos.col - scan_line + dist_limit)));

		for (multiset<interval>::iterator iit = low_it; iit != high_it; )
		{
			if (residual.find(iit->center) == residual.end() || !iit->check_available(scan_line, dist_limit))
			{
				multiset<interval>::const_iterator tmp_it = iit++;
				interval_set.erase(tmp_it);
			}
			else
			{
				if  (iit->check_inside(scan_line, dist_limit, cur_pos.col))
				{
					process_pair(iit->center, cur_pos);
				}
				++iit;
			}
		}

		interval_set.insert(interval(cur_pos));
	}
}

void Grammar_transformer::process_pair(position p1, position p2)
{
	int l1 = label_map[p1];
	int l2 = label_map[p2];

	variable cur_var(p2.row - p1.row, p2.col - p1.col, l1, l2);

	var_map_it cur_var_it = var_map.find(cur_var);
	int var_num;
	if (cur_var_it == var_map.end()) var_num = -1;
	else var_num = cur_var_it->second;

	
	// case 1: (p1, p2) can be extracted as a variable that already exist
	if (var_num > 0)
	{
		remove_position(p1);
		label_map[p2] = var_num;
		return;
	}
	
	// case 2: variable corresponding to (p1, p2) already in var_cand_map
	// case 2.1: older var_cand is no longer available, use (p1, p2) to update it
	// case 2.2: (p1, p2) can be extracted together with a pair of position (p3, p4) in var_cand_map 
	var_cand_map_it var_cand_it = var_cand_map.find(cur_var);
	if (var_cand_it != var_cand_map.end())
	{
		const position & p3 = var_cand_it->second.first;
		const position & p4 = var_cand_it->second.second;
		if (residual.find(p3) == residual.end() || residual.find(p4) == residual.end() || 
			label_map[p3] != cur_var.label1 || label_map[p4] != cur_var.label2 || 
			p1 == p4)
		{
			var_cand_map[cur_var] = pair<position, position>(p1, p2);
			return;
		}

		var_map[cur_var] = ++var_counter;
		remove_position(p3);
		remove_position(p1);
		label_map[p2] = var_counter;
		label_map[p4] = var_counter;
		return;
	}

	// case 3: otherwise, add (p1, p2) to var_cand_map
	var_cand_map.emplace(cur_var, pair<position, position>(p1, p2));
}

void Grammar_transformer::remove_position(position p)
{
	label_map.erase(p);
	residual.erase(p);
}

void Grammar_transformer::init_residual()
{
	string file_name = file_stem + ".coo";
	ifstream fin;
	fin.open(file_name);

	residual.clear();
	label_map.clear();
	
	int cnt;
	fin >> height >> width >> cnt;
	int row, col;
	while (fin >> row >> col)
	{
		position p(row, col);
		residual.insert(p);
		label_map.emplace(p, 0);
	}
	fin.close();
}

void Grammar_transformer::output_raw_grammar()
{
	string file_name = file_stem + ".raw";
	ofstream fout;
	fout.open(file_name);

	fout << height << " " << width << " " << var_map.size()<< endl;
	for (residual_it it = residual.begin(); it != residual.end(); ++it)
		fout << (height - it->row) << " " << (width - it->col) << " " << label_map[*it] << " ";
	fout << endl;

	var_map_it *it_list = new var_map_it [var_map.size()+1];
	for (var_map_it it = var_map.begin(); it != var_map.end(); ++it)
		it_list[it->second] = it;

	for (int i=1; i<=var_map.size();++i)
	{
		var_map_it it = it_list[i];

		const variable &tmp = it->first;
		fout << tmp.label2 << " " << tmp.delta_row << " " << tmp.delta_col << " " << tmp.label1 << endl;
	}

	delete [] it_list;
	fout.close();
}
