#ifndef INTERVAL_H
#define INTERVAL_H

#include "position.h"

class interval
{
public:
	position center;

	interval(position p);
	interval();

	bool check_inside(int scan_line, int dist_limit, int col) const;
	bool check_available(int scan_line, int dist_limit) const;

	interval& operator=(const interval& p);
	bool operator<(const interval& p) const;
	bool operator==(const interval& p) const;
};


#endif
