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
	template<class BidirectionalIterator1, class BidirectionalIterator2>
	inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result)
	{
		while (first != last)
		{
			*(--result) = *(--last);
		}
		return result;
	}
	//fill&fill_n
	template<class ForwardInterator, class T>
	void fill(ForwardInterator first, ForwardInterator last, const T& value)
	{
		for (; first != last; ++first)
			*first = value; //直接简单拷贝
	}
	template<class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		for (; n > 0; --n, ++first)
			*first = value;
		return first;
	}
	//对(unsigned)char的特殊处理
	inline void fill(unsigned char* first, unsigned char* last, const unsigned char& c)
	{
		unsigned char tmp = c;
		memset(first, tmp, last - first);
	}

	inline void fill(signed char* first, signed char* last, const signed char& c)
	{
		signed char tmp = c;
		memset(first, static_cast<unsigned char>(tmp), last - first);
	}

	inline void fill(char* first, char* last, const  char& c)
	{
		char tmp = c;
		memset(first, static_cast<unsigned char>(tmp), last - first);
	}
	template<class T>
	T max(T a, T b)
	{
		return (a > b ? a : b);
	}
	template<class T>
	inline void swap(T&  a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}
	template <class InputIterator1, class InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1&&first2 != last2)
		{
			if (*first1 < *first2)
			{
				return true;
			}
			if (*first1 > *first2)
			{
				return false;
			}
		}
		return first1 == last1&&first2 != last2;
	}

	template <class InputIterator1, class InputIterator2, class Compare>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		Compare comp)
	{
		while (first1 != last1&&first2 != last2)
		{
			if (comp(*first1, *first2))
			{
				return true;
			}
			if (comp(*first2, *first1))
			{
				return false;
			}
		}
		return first1 == last1&&first2 != last2;

	}
}
#endif