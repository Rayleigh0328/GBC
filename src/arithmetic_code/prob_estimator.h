/*
 * Note: This code is specially tuned for code labels. Data i always appear after i-1 and the data starts from 1.

 * Probability estimator class.
 * Member variables:
 * 	alphabet_size: denote the size of alphabet. Alphabet contains a special symbol with index 0, which indicate the end of sequence.
 * 	count: array used to count the appearance of each symbol
 * 	tot_appear: sum of array count
 *
 * Member functions:
 * 	process_symbol(ind): update count and tot_appear after a symbol coming.
 * 	query_lb(ind): return sum 0 to ind-1 of array count.
 * 	query_ub(ind): return sum 0 to ind of array count.
 * 	query_tot() return tot_appear.
 * 	find_symbol(value, left, len): accoding to the position of value in interval represented by left and len, find correspoding symbol.
 */

#ifndef PROB_ESTIMATOR
#define PROB_ESTIMATOR

#include<iostream>
#include<cstdio>
#include<vector>

using namespace std;

class Prob_estimator{
	private:
		vector<int> count;
		int tot_appear;
		int capacity;

		static const int INIT_CAPACITY = 2;
	public:
		Prob_estimator();
		~Prob_estimator();
		void process_symbol(int index);
		int query_lb(int index);
		int query_ub(int index);
		int query_tot();
		int find_symbol(int value, int left, int len);
};


#endif
