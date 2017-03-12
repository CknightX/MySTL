#ifndef _CK_UNINITIALIZED_H
#define _CK_UNINITIALIZED_H
#include<cstring>
#include "ck_construct.h"
#include "ck_type_traits.h"
#include "ck_iterator.h"
#include "ck_algorithm.h"

namespace CK_STL
{
	template<class InputIterator ,class ForwardInterator>
	ForwardInterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardInterator result, _true_type)
	{
		return copy(first, last, result);
	}
	template<class InputIterator, class ForwardInterator>
	ForwardInterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardInterator result, _false_type)
	{
		ForwardInterator cur = result;
		for (; first != last; ++cur)
		{
			construct(&*cur, *first);
		}
		return cur;
	}
	template<class InputIterator, class ForwardInterator>
	ForwardInterator uninitialized_copy(InputIterator first, InputIterator last, ForwardInterator result)
	{
		typedef typename _type_traits<iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
		return _uninitialized_copy_aux(first, last, result, isPODType());
	}

	//[first,last)��������ݿ�����result��ʼ���ڴ棬����β�������
	//const char* �ػ��汾
	inline char* uninitialized_copy(const char *first, const char *last, char *result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}
	inline wchar_t* uninitialized_copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}




	/************************************************************************/
	/*	��[first,last)�����ڵ�������x���
		1.�����������Ͷ�����ͬ��������
		*/
	/************************************************************************/
	template<class ForwardInterator, class T>
	void _uninitialized_fill_aux(ForwardInterator first, ForwardInterator last, const T& value, _true_type)
	{
		fill(first, last, value);
	}
	template<class ForwardInterator, class T>
	void _uninitialized_fill_aux(ForwardInterator first, ForwardInterator last, const T& value, _false_type)
	{
		for (; first != last; ++first)
		{
			construct(&*first, x);
		}
	}
	template<class ForwardInterator, class T>
	void uninitialized_fill(ForwardInterator first, ForwardInterator last, const T& x)
	{
		typedef typename _type_traits<T>::is_POD_type isPODType;
		_uninitialized_fill_aux(first, last, x, isPODType());
	}
	/************************************************************************/
	/*	��first��ʼ��n�����ݿ���x�����
		1.�����������Ͷ�����ͬ��������
		*/
	/************************************************************************/
	template<class ForwardInterator, class Size, class T>
	ForwardInterator _uninitialized_fill_n_aux(ForwardInterator first, Size n, const T& x, _true_type)
	{
		return fill_n(first, n, x);
	}
	template<class ForwardInterator, class Size, class T>
	ForwardInterator _uninitialized_fill_n_aux(ForwardInterator first, Size n, const T& x, _false_type)
	{
		int i = 0;
		for (; i != n; ++i)
		{
			construct((T*)(first + i), x);
		}
		return (first + i);
	}

	template<class ForwardInterator, class Size, class T>
	inline ForwardInterator uninitialized_fill_n(ForwardInterator first, Size n, const T& x)
	{
		typedef typename _type_traits<T>::is_POD_type isPODType;
		return _uninitialized_fill_n_aux(first, n, x, isPODType());
	}
}
#endif