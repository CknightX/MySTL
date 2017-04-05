#ifndef _CK_PRIORITY_QUEUE_H
#define _CK_PRIORITY_QUEUE_H
#include "ck_vector.h"
#include "ck_heap.h"
#include "ck_functional.h"
namespace CK_STL
{
	template<class T,class Sequence=vector<T>,class Compare=less<typename Sequence::value_type> >
		class priority_queue
		{
			public:
				typedef typename Sequence::value_type value_type;
				typedef typename Sequence::size_type size_type;
				typedef typename Sequence::reference reference;
				typedef typename Sequence::const_reference const_reference;
			protected:
				Sequence c; //Container
				Compare comp; //the standard of compare
			public:
				priority_queue():c(){}
				explicit priority_queue(const Compare& x):c(),comp(x){}

				template<class InputIterator>
					priority_queue(InputIterator first,InputIterator last,const Compare& x=Compare())
					:c(first,last),comp(x){heap_construct::make_heap(c.begin(),c.end(),comp);}
				~priority_queue(){}
			public:
				bool empty()const{return c.empty();}
				size_type size(){return c.size();}
				const_reference top()const{return c.front();}
				void push(const value_type& x)
				{
					c.push_back(x);
					heap_construct::push_heap(c.begin(),c.end(),comp);
				}
				void pop()
				{
					heap_construct::pop_heap(c.begin(),c.end(),comp);
					c.pop_back();
				}
		};
}
#endif
