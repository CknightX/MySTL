#ifndef _CK_CONSTRUCT_H
#define _CK_CONSTRUCT_H
#include <new.h>
template<class T1,class T2>
inline void construct(T1* p, const T2& value)
{
	new (p)T1(value); // placement new ��p�ϵ���T1::T1(value)
}

template<class T1>
inline void construct(T1* p)
{
	new(p)T1();
}
template<class T>
inline void destroy(T* pointer)
{
	pointer->~T();
}
//�����������������跨�ҳ�Ԫ�ص���ֵ�ͱ�
//Ȼ����_type_traits<>��ȡ���ʵ���ʩ
template<class ForwardInterator>
inline void destroy(ForwardInterator first, ForwardInterator last)
{
	_destroy(first, last, value_type(first));
}

template<class ForwardInterator>
inline void _destroy(ForwardInterator first, ForwardInterator last, T*)
{
	typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor;
	_destroy_aux(first, last, trivial_destructor());
}

template<class ForwardInterator>
inline void _destroy_aux(ForwardInterator first, ForwardInterator last, _false_type)
{
	for (; first < last; ++first)
		destroy(&*first); //������->ԭ��ָ��
}
template<class ForwardInterator>
inline void _destroy_aux(ForwardInterator, ForwardInterator, _true_type);

inline void destroy(char*, char*){}
inline void destroy(wchar_t*, wchar_t*){}
#endif