#include "test_func.h"
#include "../ck_heap.h"
#include<vector>
using std::vector;
namespace CK_STL
{
	namespace TEST
	{
		int ia[9]={0,1,2,3,4,8,9,3,5};
		vector<int> ivec(ia,ia+9);
		static void test1()
		{

			heap_construct::make_heap(ivec.begin(),ivec.end());
			print(ivec);

			ivec.push_back(7);
			heap_construct::push_heap(ivec.begin(),ivec.end());
			print(ivec);

			heap_construct::pop_heap(ivec.begin(),ivec.end());
			std::cout<<ivec.back()<<std::endl;
			ivec.pop_back();

			print(ivec);

			heap_construct::sort_heap(ivec.begin(),ivec.end());
			print(ivec);

		}
		static void test2()
		{
			heap_construct::make_heap(ivec.begin(),ivec.end());
			heap_construct::pop_heap(ivec.begin(),ivec.end());
			std::cout<<ivec.back()<<std::endl;
			ivec.pop_back();
			heap_construct::pop_heap(ivec.begin(),ivec.end());
			std::cout<<ivec.back()<<std::endl;
			ivec.pop_back();
			heap_construct::pop_heap(ivec.begin(),ivec.end());
			std::cout<<ivec.back()<<std::endl;
			ivec.pop_back();
			heap_construct::pop_heap(ivec.begin(),ivec.end());
			std::cout<<ivec.back()<<std::endl;
			ivec.pop_back();
			
		}
			void heap_test()
			{
				test1();
			}


	}
}
