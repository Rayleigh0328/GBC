#include "ac_decoder.h"

int AC_decoder::get_bit(queue<bool>& buffer)
{
	if (cnt++ >= code_word_length) return 0;

	int ans = (int)buffer.front();
	buffer.pop();
	return ans;
}

AC_decoder::AC_decoder(int precision)
{
	b = precision;
	left = 0;
	len = TOP_VALUE;

	cnt = 0;
}

AC_decoder::~AC_decoder()
{}

void AC_decoder::update_value(queue<bool> &input)
{
	while (len < FIRST_QTR || left >= HALF || left+len < HALF)
	{
		if (left + len < HALF)
		{}
		else
		{
			if (left >= HALF)
			{
				value -= HALF;
				left -= HALF;
			}
			else
			{
				value -= FIRST_QTR;
				left -= FIRST_QTR;
			}
		}
		left = left << 1;
		len = (len << 1) + 1;
		value = (value << 1) + get_bit(input);
	}
}

void AC_decoder::decode(queue<bool>&input, vector<int> &output)
{
	Elias_decoder int_dec(input);
	vector<int> tmp;
	int_dec.decode(tmp);

	code_word_length = tmp[0];

	//printf("dec code word length: %d\n", code_word_length);

	value = 0;
	for (int i=0;i<b;++i)
		value = (value<<1) + get_bit(input);

	int index;
	do
	{
		index = pe.find_symbol(value,left,len);

		output.push_back(index);

		int lb = pe.query_lb(index);
		int ub = pe.query_ub(index);
		int tot = pe.query_tot();
		int tmp = ((long long) len * lb) / tot;
		left += tmp;
		len = (int) ((long long) len * ub / tot - tmp);
		update_value(input);
		pe.process_symbol(index);
	}
	while (index != 0);
}

