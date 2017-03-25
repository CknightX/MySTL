/*
为alloc提供标准接口
*/
#ifndef _CK_SIMPLE_ALLOC_H
#define _CK_SIMPLE_ALLOC_H
namespace CK_STL
{
	template < class T, class Alloc >
	class simple_alloc
	{
	public:
		static T* allocate(size_t n)
		{
			return n == 0 ? nullptr : static_cast<T*>(Alloc::allocate(n*sizeof(T)));
		}
		static T* allocate(void)
		{
			return static_cast<T*>(Alloc::allocate(sizeof(T)));
		}
		static void deallocate(T *p, size_t n)
		{
			if (n != 0)
			{
				Alloc::deallocate(p, n*sizeof(T));
			}
		}
		static void deallocate(T *p)
		{
			deallocate(p, 1);
		}
	};

}
#endif
