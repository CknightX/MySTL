#include "p_queue_test.h"
#include <queue>
#include<iostream>
#include <algorithm>
#include "../ck_priority_queue.h"
namespace CK_STL
{
	namespace TEST
	{
		static void test1()
		{
			int ia[9]={0,1,2,3,4,8,9,3,5};
			priority_queue<int> ipq(ia,ia+9);
			std::cout<<"size="<<ipq.size()<<std::endl; //size=9

			for (int i=0;i<ipq.size();++i)
				std::cout<<ipq.top()<<' '; // 9 9 9 9 9 9 9 9 9
			std::cout<<std::endl;

			while(!ipq.empty())
			{
				std::cout<<ipq.top()<<' '; // 9 8 5 4 3 3 2 1 0
				ipq.pop();
			}
			std::cout<<std::endl;
		}
		void p_queue_test()
		{
			test1();
		}
	}
}
