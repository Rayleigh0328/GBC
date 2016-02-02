#include "interval.h"

interval::interval(){}

interval::interval(position p)
{
	center = p;
}

bool interval::check_inside(int scan_line, int limit, int col) const 
{
	// check whether point col located in the interval,
	// when scan line miove to $scan_line and dist limit is $limit.

	int r0 = center.row;
	int c0 = center.col;
	int delta = limit - scan_line + r0;
	return (c0 - delta <= col && col <= c0 + delta);
}

bool interval::check_available(int scan_line, int limit) const 
{
	// Check whether the interval exist or not, 
	// when scan line move to $scan_line and dist limit is $limit.

	return (scan_line - center.row <= limit);
}

interval& interval::operator=(const interval &p)
{
	center = p.center;
	return *this;
}

bool interval::operator<(const interval &p) const
{
	if (center.col - center.row < p.center.col - p.center.row) return true;
	return false;
}

bool interval::operator==(const interval &p) const
{
	return (center.col- center.row == p.center.col - p.center.row); 
}
