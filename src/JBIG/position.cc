#include "position.h"

position::position()
{
	row = col = 0;
}

position::position(int r, int c)
{
	row = r;
	col = c;
}

position &position::operator=(const position &p)
{
	row = p.row;
	col = p.col;
	return *this;
}

bool position::operator<(const position &other) const 
{
	if (row < other.row) return true;
	if (row > other.row) return false;

	if (col < other.col) return true;

	return false;
}

bool position::operator==(const position &other) const
{
	return (row == other.row && col == other.col);
}

