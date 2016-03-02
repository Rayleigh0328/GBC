#include "quad_tree_transformer.h"

Quad_tree_transformer::Quad_tree_transformer(){}

double Quad_tree_transformer::transform(string file_name, int tile_size)
{
	ifstream fin;
	fin.open(file_name);

	node *root = new node;
	int cnt;
	
	string tmp;
	while (fin.peek() == '%') getline(fin, tmp);

	fin >> root->height >> root->width >> cnt;
	root->ps.clear();
	for (int i=0;i<cnt;++i)
	{
		int r, c;
		fin >> r >> c;
		getline(fin, tmp);
		root->ps.insert(position(r-1, c-1));
	}
	for (int i=0;i<4;++i)
		root->child[i] = NULL;

	split(root, tile_size);

	fin.close();
/*
	ofstream fout;
	string check_filename = "check.mtx";
	fout.open(check_filename);

	transform_back(root, tile_size, fout);
	fout.close();
*/
	return get_complexity(root, tile_size);
}

void Quad_tree_transformer::transform_back(node* root, int tile_size, ofstream &fout)
{
	set<position> ps;
	ps.clear();
	construct_ps(root,tile_size,ps,0,0);
	fout << root->height << " " << root->width << " " << ps.size() << endl;
	for (pit it= ps.begin(); it != ps.end(); ++it)
		fout << it->row + 1<< " " << it->col + 1<< endl;
}

void Quad_tree_transformer::construct_ps(node* p, int tile_size, set<position>& ans, int hb, int wb)
{
	if (p==NULL) return;

	if (p->height < tile_size && p->width < tile_size)
		for (pit it = p->ps.begin(); it != p->ps.end(); ++it)
			ans.insert(position(hb + it->row, wb + it->col));
	
	construct_ps(p->child[0], tile_size, ans, hb, wb);
	construct_ps(p->child[1], tile_size, ans, hb, wb + p->width/2 + 1);
	construct_ps(p->child[2], tile_size, ans, hb + p->height/2 + 1, wb);
	construct_ps(p->child[3], tile_size, ans, hb + p->height/2 + 1, wb + p->width/2 + 1);
}

void Quad_tree_transformer::split(node* p, int tile_size)
{
	// if both width and height within tile_size stop splitting
	if (p == NULL || p->height <= tile_size && p->width <= tile_size) return;

	// split into 4 area
	int dr = p->height / 2;
	int dc = p->width / 2;

	for (pit it = p->ps.begin(); it != p->ps.end(); )
	{
		cpit tmp_it = it;
		if (it->row <= dr && it->col <= dc) process_point(p, 0, it->row, it->col);
		if (it->row <= dr && it->col >  dc) process_point(p, 1, it->row, it->col - dc - 1);
		if (it->row >  dr && it->col <= dc) process_point(p, 2, it->row - dr - 1, it->col);
		if (it->row >  dr && it->col >  dc) process_point(p, 3, it->row - dr - 1, it->col - dc - 1);

		++it;
		p->ps.erase(tmp_it);
	}
	
	// check ps should be empty
	if (!p->ps.empty())
	{
		cerr << "point set not empty" << endl;
		while(1);
	}

	for (int i=0;i<4;++i)
		split(p->child[i], tile_size);
}

void Quad_tree_transformer::process_point(node* p, int c_num, int row, int col)
{
	if (p->child[c_num]==NULL)
	{
		p->child[c_num] = new node;

		if (c_num % 2 == 0) p->child[c_num]->width = (p->width + 1)/2;
		else p->child[c_num]->width = p->width - (p->width +1)/2;

		if (c_num < 2) p->child[c_num]->height = (p->height + 1)/2;
		else p->child[c_num]->height = p->height - (p->height + 1)/2;

		p->child[c_num]->ps.clear();
		for (int i=0;i<4;++i)
			p->child[c_num]->child[i] = NULL;
	}
	
	p->child[c_num]->ps.insert(position(row, col));
}

int Quad_tree_transformer::get_byte(int t)
{
	for (int i=8;i<=64;i+=8)
		if (t<= (1<<i)) return i/8;
	return 4;
}

int Quad_tree_transformer::get_complexity(node* p, int tile_size)
{
	if (p == NULL) return 0;
	int ans;

	if (p->height <= tile_size && p->width <= tile_size)
	{
		int tb = get_byte(tile_size);

		// COO
		int size_coo = p->ps.size() * 2 * tb;

		// CRS
		int size_crs = p->ps.size() * tb + p->height * get_byte(p->ps.size());

		ans = min(size_coo, size_crs);
		//cerr << "heihgt: " << p->height << endl;
		//cerr << "width: " << p->width << endl;
		//cerr << "# of points: "  << p->ps.size() << endl; 
		//cerr << size_coo <<  " " << size_crs << endl;
		return ans + 1;
	}

	ans = 1;
	for (int i=0;i<4;++i)
		ans += get_complexity(p->child[i], tile_size);
	return ans;
}



