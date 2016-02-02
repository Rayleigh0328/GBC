/*
 * AC encoder class.
 * Note that symbol with index 0 is a special symbol denoting the end of input sequence.
 *
 * Member:
 * 	left, len: keep track on the interval.
 * 	pe: probablity estimator.
 * 	k: bits that borrowed
 * 	b: precision.
 *
 * Method:
 * 	incremental_output(): when we can determine some bits of codeword, output them immediately. Thus, we can avoid precision problem.
 * 	output_bit_and_following(): output bits according to value of k
 * 	encode_symbol(): encode a symbol
 */

#ifndef AC_ENCODER_H
#define AC_ENCODER_H

#include <iostream>
#include <cstdio>
#include <queue>

#include "ac_def.h"
#include "prob_estimator.h"
#include "../elias_code/elias_encoder.h" 

using namespace std;

class AC_encoder{
	private:
		unsigned int left, len;
		Prob_estimator pe;
		int k;
		int b;
		vector<bool> buffer;

		void incremental_output();
		void output_bit_and_following(int bit);
		void end_encode(queue<bool> & output);

	public:
		AC_encoder(int precision = 31);
		~AC_encoder();

		void encode_symbol(int index, queue<bool> & output);
};

#endif
