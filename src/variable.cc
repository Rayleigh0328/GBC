#include "variable.h"

variable::variable(int dr, int dc, int l1, int l2)
{
	delta_row = dr;
	delta_col = dc;
	label1 = l1;
	label2 = l2;
}

bool variable::operator==(const variable &other) const
{
	return (delta_row == other.delta_row && delta_col == other.delta_col && label1 == other.label1 && label2 == other.label2);
}
