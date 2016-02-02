/*
 * AC decoder class.
 * Member variables:
 * 	left, len: used to keep track on the current interval
 * 	value: denote the point used to decode
 * 	pe: probability estimator class.
 * 	buffer: input buffer.
 * 	b: precision, number of bits we used to represent interval and value
 *
 * Method:
 * 	update_value(): incremental update value and interval. Corresponding to the incremental part in encoder.
 * 	decode(): decode symbol, then update value and interval according to the result.
 */

#ifndef AC_DECODER_H
#define AC_DECODER_H

#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>

#include "ac_def.h"
#include "prob_estimator.h"
#include "../elias_code/elias_decoder.h"

class AC_decoder{
	private:
		unsigned int left, len, value;
		Prob_estimator pe;
		int b;
		bool padding_flag;
		int cnt;
		int code_word_length;

		void update_value(queue<bool>& buffer);
		int get_bit(queue<bool>& buffer);

	public:
		AC_decoder(int precision = 31);
		~AC_decoder();

		void decode(queue<bool>&input, vector<int> &output);
};


#endif
