#include "huffman_codec.h"

huffman_codec::huffman_codec(int alphabet_size)
{
	// allocate memory for arrays
	n = alphabet_size;
	z = 2*n - 1;

	alpha = new int [n + 1];
	rep = new int [n + 1];

	block = new int [z + 1];
	weight = new int [z + 1];
	parent = new int [z + 1];
	parity = new int [z + 1];
	rt_child = new int [z + 1];
	first = new int [z + 1];
	last = new int [z + 1];
	prev_block = new int [z + 1];
	next_block = new int [z + 1];

	// initialize m,r, alpha[], rep[]
	m = e = 0;
	r = -1;

	for (int i=1;i<=n;++i)
	{
		++m;++r;
		if (2*r == m)
		{
			++e;
			r = 0;
		}
		alpha[i] = rep[i] = i;
	}

	// initialize node n as the first node in the huffman tree
	block[n] = 1;
	prev_block[1] = next_block[1] = 1;
	weight[1] = 0;
	first[1] = last[1] = n;
	parity[1] = 0;
	parent[1] = 0;

	// initialize availbale block list
	avail_block = 2;
	for (int i=avail_block; i<z; ++i)
		next_block[i] = i + 1;
	next_block[z] = 0;
}

huffman_codec::~huffman_codec()
{
	delete [] alpha;
	delete [] rep;
	delete [] block;
	delete [] weight;
	delete [] parent;
	delete [] rt_child;
	delete [] first;
	delete [] last;
	delete [] prev_block;
	delete [] next_block;
}

void huffman_codec::encode(int x, queue<bool>& output)
{
	encode_and_transmit(x, output);
	update(x);
}

void huffman_codec::decode(queue<bool>& buf, vector<int>& result)
{
	int tmp;
	while (!buf.empty())
	{	
		tmp = receive_and_decode(buf);
		result.push_back(tmp);
		update(tmp);
	}
}

int huffman_codec::decode(queue<bool>& buf)
{
	int tmp =  receive_and_decode(buf);
	update(tmp);
	return tmp;
}

void huffman_codec::encode_and_transmit(int j, queue<bool>& output)
{
	int it, q, t, root;
	q = rep[j];
	if (q <= m)
	{
		if (--q < 2*r) t = e + 1;
		else
		{
			q = q-r;
			t = e;
		}
		for (it = 0; it<t; ++it)
		{
			encode_stack.push((q&1)?true:false);
			q = q >> 1;
		}
		q = m;
	}
	if (m == n) 
		root = n;
	else 
		root = z;

	while (q != root)
	{
		encode_stack.push(((first[block[q]] - q + parity[block[q]]) & 1)?true:false);
		q = parent[block[q]] - ((first[block[q]] - q + 1 - parity[block[q]]) >> 1);
	}
	
	while (!encode_stack.empty())
	{
		output.push(encode_stack.top());
		encode_stack.pop();
	}
}

int huffman_codec::receive_and_decode(queue<bool>& buf)
{
	int i, q;
	if (m == n) q = n;
	else q = z;

	while (q > n) 
	{
		q = find_child(q, (buf.front()?1:0));
		buf.pop();
	}
	if (q == m)
	{
		q = 0;
		for (i = 0; i<e; ++i)
		{
			q = 2*q + (buf.front()?1:0);
			buf.pop();
		}

		if (q < r)  
		{
			q = 2*q + (buf.front()?1:0);
			buf.pop();

		}
		else q = q + r;

		++q;
	}	
	return alpha[q];
}

int huffman_codec::find_child(int j, int parity)
{
	int delta = 2 * (first[block[j]] - j) + 1 - parity;
	int right = rt_child[block[j]];
	int gap = right - last[block[right]];

	if (delta <= gap)
	{
		return right - delta;
	}
	else
	{
		delta = delta - gap - 1;
		right = first[prev_block[block[right]]];
		gap = right - last[block[right]];
		if (delta <= gap) 
			return right - delta;
		else
			return first[prev_block[block[right]]] - delta + gap + 1;
	}
}


void huffman_codec::interchange_leaves(int e1, int e2)
{
	rep[alpha[e1]] = e2;
	rep[alpha[e2]] = e1;
	int tmp = alpha[e1];
	alpha[e1] = alpha[e2];
	alpha[e2] = tmp;
}

void huffman_codec::update(int k)
{
	int q, leaf_to_increment, bq, b, old_parent, old_parity, nbq, par, bpar;
	bool slide;

	//find node
	q = rep[k];
	leaf_to_increment = 0;
	if (q<=m)
	{
		interchange_leaves(q,m);
		if (r==0) 
		{
			r = m >> 1;
			if (r > 0) --e;
		}
		--m;--r;
		q = m + 1;
		bq = block[q];
		if (m > 0)
		{
			block[m] = bq;
			last[bq] = m;
			old_parent = parent[bq];
			parent[bq] = m + n;
			parity[bq] = 1;

			b = avail_block;
			avail_block = next_block[avail_block];
			prev_block[b] = bq;
			next_block[b] = next_block[bq];
			prev_block[next_block[bq]] = b;
			next_block[bq] = b;
			parent[b] = old_parent;
			parity[b] = 0;
			rt_child[b] = q;
			block[m+n] = b;
			weight[b] = 0;
			first[b] = last[b] = m+n;
			leaf_to_increment = q;
			q = m + n;
		}
	}
	else
	{
		interchange_leaves(q,first[block[q]]);
		q = first[block[q]];
		if (q == m + 1 && m > 0)
		{
			leaf_to_increment = q;
			q = parent[block[q]];
		}
	}
	// end of find node
	

	while (q>0)
		slide_and_increment(q, leaf_to_increment, bq, b, old_parent, old_parity, nbq, par, bpar, slide);
	if (leaf_to_increment != 0)
	{
		q = leaf_to_increment;
		slide_and_increment(q, leaf_to_increment, bq, b, old_parent, old_parity, nbq, par, bpar, slide);
	}
}

void huffman_codec::slide_and_increment(int &q, int &leaf_to_increment, int &bq, int &b, 
	int &old_parent, int &old_parity, int &nbq, int &par, int &bpar, bool &slide)
{
	bq = block[q];
	nbq = next_block[bq];
	par = parent[bq];
	old_parent = par;
	old_parity = parity[bq];

	if (((q<=n) && (first[nbq]>n) && (weight[nbq] == weight[bq])) ||
		((q>n) && (first[nbq]<=n) && (weight[nbq] == weight[bq] + 1)))
	{
		slide = true;
		old_parent = parent[nbq];
		old_parity = parity[nbq];
		if (par > 0)
		{
			bpar = block[par];
			if (rt_child[bpar] == q) rt_child[bpar] = last[nbq];
			else 
				if (rt_child[bpar] == first[nbq]) rt_child[bpar] = q;
				else rt_child[bpar] = rt_child[bpar] + 1;
			if (par != z)
				if (block[par + 1] != bpar)
					if (rt_child[block[par + 1]] == first[nbq])
						rt_child[block[par + 1]] = q;
					else if (block[rt_child[block[par+1]]] == nbq)
						rt_child[block[par + 1]] = rt_child[block[par+1]] + 1;
		}
		parent[nbq] = parent[nbq] - 1 + parity[nbq];
		parity[nbq] = 1 - parity[nbq];
		nbq = next_block[nbq];
	}
	else
		slide = false;
		
	if ( (((q<=n) && (first[nbq]<=n)) || ((q>n) && (first[nbq]>n)))  && (weight[nbq] == weight[bq] + 1) )
	{
		block[q] = nbq;
		last[nbq] = q;
		if (last[bq] == q)
		{
			next_block[prev_block[bq]] = next_block[bq];
			prev_block[next_block[bq]] = prev_block[bq];
			next_block[bq] = avail_block;
			avail_block = bq;
		}
		else
		{
			if (q>n) rt_child[bq] = find_child(q-1,1);
			if (parity[bq] == 0) --parent[bq];
			parity[bq] = 1 - parity[bq];
			first[bq] = q - 1;
		}
	}
	else 
		if (last[bq] == q)
		{
			if (slide)
			{
				prev_block[next_block[bq]] = prev_block[bq];
				next_block[prev_block[bq]] = next_block[bq];
				prev_block[bq] = prev_block[nbq];
				next_block[bq] = nbq;
				prev_block[nbq] = bq;
				next_block[prev_block[bq]] = bq;
				parent[bq] = old_parent;
				parity[bq] = old_parity;
			}
			++weight[bq];
		}
		else
		{
			b = avail_block;
			avail_block = next_block[avail_block];
			block[q] = b;
			first[b] = q;
			last[b] = q;
			if (q>n)
			{
				rt_child[b] = rt_child[bq];
				rt_child[bq] = find_child(q-1, 1);
				if (rt_child[b] == q-1) parent[bq] = q;
				else if (parity[bq] == 0) parent[bq] = parent[bq] - 1;
			}
			else if (parity[bq] == 0) --parent[bq];
			first[bq] = q - 1;
			parity[bq] = 1 - parity[bq];
			
			// insert q's block into block list
			prev_block[b] = prev_block[nbq];
			next_block[b] = nbq;
			prev_block[nbq] = b;
			next_block[prev_block[b]] = b;
			weight[b] = weight[bq] + 1;
			parent[b] = old_parent;
			parity[b] = old_parity;
		}

	// move q 1-level higher
	if (q<=n) q = old_parent;
	else q = par;
}
