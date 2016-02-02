#include "ac_encoder.h"

AC_encoder::AC_encoder(int precision)
{
	b = precision;
	left = 0;
	len = TOP_VALUE;
	k = 0;
}

AC_encoder::~AC_encoder()
{}

void AC_encoder::output_bit_and_following(int bit)
{
	buffer.push_back((bool)bit);
	for (int i=0;i<k;++i)
		buffer.push_back((bool)1-bit);
}

void AC_encoder::incremental_output()
{
	while (len < FIRST_QTR || left >= HALF || left+len <HALF)
	{
		if (left+len < HALF)
		{
			output_bit_and_following(0);
			k = 0;
		}
		else
		{
			if (left >=HALF)
			{
				output_bit_and_following(1);
				k = 0;
				left -= HALF;
			}
			else
			{
				k++;
				left -= FIRST_QTR;
			}
		}

		left = left << 1;
		len = (len << 1) + 1;
	}
}

void AC_encoder::end_encode(queue<bool> &output)
{
	++k;
	if (left < FIRST_QTR)
		output_bit_and_following(0);
	else 
		output_bit_and_following(1);

	Elias_encoder int_enc;
	int_enc.encode(buffer.size(), output);

	//printf("enc code word length: %d\n", buffer.size());

	for (int i=0;i<buffer.size();++i)
		output.push(buffer[i]);

	buffer.clear();
}

void AC_encoder::encode_symbol(int index, queue<bool> &output)
{
	int lb = pe.query_lb(index);
	int ub = pe.query_ub(index);
	int tot = pe.query_tot();

	int tmp = ((long long)len * lb)/tot;
	left += tmp;
	len = (int) (((long long)len * ub)/tot - tmp);

	incremental_output();
	pe.process_symbol(index);

	if (index == 0) end_encode(output);
}

