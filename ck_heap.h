#ifndef _CK_HEAP_H
#define _CK_HEAP_H
#include "ck_iterator.h"
namespace CK_STL
{
	class heap_construct
	{
		public:
			template<class RandomAccessIterator,class Distance,class T>
			static void push_heap(RandomAccessIterator first,Distance hole_index,Distance top_index,T value)
				{
					typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
					typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;
					difference_type parent=(hole_index-1)/2;
					while(hole_index > top_index && *(first+parent)<value)
					{
						*(first+hole_index)=*(first+parent);
						hole_index=parent;
						parent=(hole_index-1)/2;
					}
					*(first+hole_index)=value;

				}
			template<class RandomAccessIterator>
				static inline void push_heap(RandomAccessIterator first,RandomAccessIterator last)
				{
					typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
					typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;

					value_type value=*(last-1);
					difference_type hole_index=static_cast<difference_type> ((last-first)-1);
					difference_type top_index=static_cast<difference_type>(0);
					push_heap(first,hole_index,top_index,value);
				}
			template<class RandomAccessIterator>
				static inline void pop_heap(RandomAccessIterator first,RandomAccessIterator last)
				{
					typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
					typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;
					value_type value=*(last-1);
					difference_type hole_index=static_cast<difference_type> (0);
					difference_type len=static_cast<difference_type> (last-first);
					RandomAccessIterator result=last-1;
					*result=*first;
					adjust_heap(first,hole_index,len-1,value);
				}
			template<class RandomAccessIterator,class Distance,class T>
				static void adjust_heap(RandomAccessIterator first,Distance hole_index,Distance len,T value)
				{
					typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
					typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;
					difference_type top_index=hole_index;
					difference_type second_child=2*hole_index+2;
					while(second_child<len)
					{
						if (*(first+second_child)<*(first+(second_child-1)))
							second_child--;
						*(first+hole_index)=*(first+second_child);
						hole_index=second_child;
						second_child=2*(second_child)+2;
					}
						if (second_child==len)  //no right child
						{
							*(first+hole_index)=*(first+(second_child-1));
							hole_index=second_child-1;
						}
						*(first+hole_index)=value;
						push_heap(first,hole_index,top_index,value);

				}
			template<class RandomAccessIterator>
				static void sort_heap(RandomAccessIterator first,RandomAccessIterator last)
				{
					while(last-first>1)
						pop_heap(first,last--);
				}
			template<class RandomAccessIterator>
				static void make_heap(RandomAccessIterator first,RandomAccessIterator last)
				{
					typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
					typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;
					if (last-first<2) return;
					difference_type len=last-first;
					difference_type hole_index=(len-2)/2; //the first node not which is not leaf node

					while(true)
					{
						adjust_heap(first,hole_index,len,static_cast<value_type>(*(first+hole_index)));
						if (hole_index==0) return;
						--hole_index;
					}

				}
		private:
	};

}
#endif
