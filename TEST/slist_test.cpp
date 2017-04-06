#include "slist_test.h"
#include "../ck_slist.h"
#include "../ck_algorithm.h"
#include <iostream>
namespace CK_STL
{
	namespace TEST
	{
		static void test1()
		{
			std::cout<<"----------slist----------"<<std::endl;
			int i;
			slist<int> islist;
			std::cout<<"size="<<islist.size()<<std::endl; //size=0

			islist.push_front(9);
			islist.push_front(1);
			islist.push_front(2);
			islist.push_front(3);
			islist.push_front(4);

			std::cout<<"size="<<islist.size()<<std::endl; //size=5

			slist<int>::iterator ite=islist.begin();
			slist<int>::iterator ite2=islist.end();
			for (;ite!=ite2;++ite)
				std::cout<<*ite<<' ';
			std::cout<<std::endl;

			ite=find(islist.begin(),islist.end(),1);
			if (ite!=0)
				islist.insert(ite,99);
			std::cout<<"size="<<islist.size()<<std::endl;
			std::cout<<*ite<<std::endl;

			ite=islist.begin();
			ite2=islist.end();
			for (;ite!=ite2;++ite)
				std::cout<<*ite<<' ';
			std::cout<<std::endl;
			ite=find(islist.begin(),islist.end(),3);
			if (ite!=0)
				std::cout<<*(islist.erase(ite))<<std::endl;
			ite=islist.begin();
			ite2=islist.end();
			for(;ite!=ite2;++ite)
				std::cout<<*ite<<' ';
			std::cout<<std::endl;
		}
		void slist_test()
		{
			test1();
		}
	}
}
