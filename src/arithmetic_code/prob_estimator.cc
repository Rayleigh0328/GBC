#include "prob_estimator.h"

Prob_estimator::Prob_estimator()
{
	capacity = INIT_CAPACITY;
	tot_appear = capacity + 1;
	count.clear();
	for (int i=0;i<=capacity;++i)
		count.push_back(1);
}

Prob_estimator::~Prob_estimator()
{
}

void Prob_estimator::process_symbol(int index)
{
	++count[index];
	++tot_appear;

	if (index == capacity)
	{
		++capacity;
		count.push_back(1);
		++tot_appear;
	}
}

int Prob_estimator::query_lb(int index)
{
	int cnt = 0;
	for (int i=0;i<index;++i)
		cnt += count[i];
	return  cnt;
}

int Prob_estimator::query_ub(int index)
{
	int cnt = 0;
	for (int i=0;i<=index;++i)
		cnt += count[i];
	return cnt;

}

int Prob_estimator::query_tot()
{
	return tot_appear;
}

int Prob_estimator::find_symbol(int value, int left, int len)
{
	int cur = 0;
	for (int i=0;i<count.size(); cur+=count[i++])
		if (((long long) value - left) * tot_appear <= (long long) (cur + count[i]) * len) return i;
}
