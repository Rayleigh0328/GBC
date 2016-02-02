#ifndef VARIABLE_H
#define VARIABLE_H

#include <functional>

using namespace std;

struct variable
{
	int delta_row;
	int delta_col;
	int label1;
	int label2;

	variable(int dr, int dc, int l1, int l2);

	bool operator==(const variable &other) const;
};

struct variable_hasher{
	size_t operator()(const variable &v) const
	{
		int tmp = v.label1;
		tmp = (tmp << 10) ^ v.label2;
		tmp = (tmp << 8) ^ v.delta_row;
		tmp = (tmp << 8) ^ v.delta_col;

		hash<int> int_hash;
		return int_hash(tmp);
	}
};
#endif

