/*
·Âº¯Êý

*/
#ifndef _CK_FUNCTIONAL_H
#define _CK_FUNCTIONAL_H
namespace CK_STL
{
	template<class T>
	struct less
	{
		bool operator()(const T& x, const T& y)const
		{
			return x < y;
		}
	};
}
#endif