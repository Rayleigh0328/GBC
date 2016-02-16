#ifndef POSITION_H 
#define POSITION_H 

#include <functional> 

using namespace std;

struct position 
{
	int row, col;

	position(int r, int c);
	position();
	position &operator=(const position &p);
	bool operator<(const position &other) const;
	bool operator==(const position &other) const;
};

struct position_hasher{
	
	size_t operator()(const position& p) const
	{
		int a = (p.row << 15)  ^ p.col;

		hash<int> int_hash;
		return int_hash(a);
	}
};
#endif
