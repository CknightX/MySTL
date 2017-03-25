#include<iostream>
#include "ck_deque.h"
using namespace CK_STL;
int main()
{
	deque<int> a;
	for (int i=0;i<100;++i)
		a.push_back(i);
	std::cout<<a.size();
	return 0;
}
