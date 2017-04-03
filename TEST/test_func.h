#ifndef _TEST_FUNC_H
#define _TEST_FUNC_H
#include<iostream>
namespace CK_STL
{
	namespace TEST
	{

		template<class InputIterator>
			void print(InputIterator first,InputIterator last)
			{
				for (;first!=last;++first)
					std::cout<<*first<<' ';
				std::cout<<std::endl;
			}
		template<class Container>
			void print(const Container& container)
			{
				print(container.begin(),container.end());
				std::cout<<std::endl;
			}
		template<class BidirectionalIterator>
			void print_reverse(BidirectionalIterator first,BidirectionalIterator last)
			{
				for(--last;last!=first;--last)
					std::cout<<*last<<' ';
				std::cout<<*first;
				std::cout<<std::endl;
			}
		template<class Container>
			void print_reverse(const Container& container)
			{
				print_reverse(container.begin(),container.end());
				std::cout<<std::endl;
			}
		template<class Container1,class Container2>
			bool container_equal(const Container1& container1,const Container2& container2)
			{
				if (container1.size()!=container2.size())
					return false;

				auto iter1=container1.begin();
				auto iter2=container2.begin();
				auto last1=container1.end();

				for (;iter1!=last1&&(*iter1==*iter2);++iter1,++iter2);

				if (iter1!=last1)
					return false;
				else return true;
			}
	}
}
#endif
