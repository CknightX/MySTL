#ifndef _CK_HEAP_H
#define _CK_HEAP_H
#include "ck_iterator.h"
namespace CK_STL
{
	class heap_construct
	{
		public:
			template<class RandomAccessIterator>
				static inline void push_heap(RandomAccessIterator first,RandomAccessIterator last)
				{
					typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
					typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;

					difference_type hole_index=static_cast<difference_type> ((last-first)-1);
					difference_type top_index=static_cast<difference_type>(0);
					difference_type parent=(hole_index)/2;
				}
			template<class RandomAccessIterator>
				static inline void pop_heap(RandomAccessIterator first,RandomAccessIterator last)
				{
					typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

				}
			template<class RandomAccessIterator,class Distance,class T>
				static void adjust_heap(RandomAccessIterator first,Distance hole_index,Distance len,T value)
				{

				}
			template<class RandomAccessIterator>
				static void sort_heap(RandomAccessIterator first,RandomAccessIterator last)
				{

				}
		private:
	};

}
#endif
