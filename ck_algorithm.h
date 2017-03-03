#ifndef _CK_ALGORITHM_H	
#define _CK_ALGORITHM_H	
#include "ck_iterator.h"
#include "ck_type_traits.h"
namespace CK_STL
{
	//----------------copy----------------
	//对外接口，完全泛化版本
	template<class InputIterator, class OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		return _copy_dispatch<InputIterator, OutputIterator>()
			(first, last, result);
	}

	//重载形式
	inline char* copy(const char *first, const char *last, char *result)
	{
		memmove(result, first, sizeof(char) * (last - first));
		return result + (last - first);
	}
	inline wchar_t* copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
	{
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}
	//_copy_dispatch
	//完全泛化
	template<class InputIterator, class OutputIterator>
	struct _copy_dispatch
	{
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
		{
			return _copy(first, last, result, iterator_category(first));
		}
	};
	// T*偏特化
	template<class T>
	struct _copy_dispatch < T*, T* >
	{
		T* operator()(T* first, T *last, T *result)
		{
			typedef typename _type_traits<T>::has_trivial_assignment_operator t;
			return _copy_t(first, last, result, t());
		}
	};
	template<class T>
	inline T* _copy_t(const T* first, const T* last, T* result, _true_type)
	{
		memmove(result, first, sizeof(T) * (last - first));
		return result + (last - first);
	}
	//InputIterator版本
	template<class InputIterator, class OutputIterator>
	inline OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
	{
		//以迭代器判断，速度慢
		for (; first != last; ++result, ++first)
		{
			*result = *first;
		}
		return result;
	}
	//RandomAccessIterator版本
	template<class RandomAccessIterator, class OutputIterator>
	inline OutputIterator _copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
	{
		return _copy_d(first, last, result, distance_type(first));
	}
	template<class RandomAccessIterator, class OutputIterator, class Distance>
	inline OutputIterator _copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*)
	{
		for (Distance n = last - first; n > 0; --n, ++result, ++first)
		{
			*result = *first;
		}
		return result;
	}

	//copy_backward
	template<class BidirectionalIterator1,class BidirectionalIterator2>
	inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result)
	{
		while (first != last)
		{
			*(--result) = *(--last);
		}
		return result;
	}
}
#endif