/*
�ռ�������
*/
#ifndef _CK_ALLOC_H
#define _CK_ALLOC_H
#include <cstdlib>
namespace CK_STL
{
	class alloc
	{
	private:
		enum { _ALIGN = 8 };
		enum { _MAX_BYTES = 128 };
		enum { _NFREELISTS = _MAX_BYTES / _ALIGN };
		union obj
		{
			union obj *next_free_list;
			char client_data[1];
		};
		static obj *free_list[_NFREELISTS];

		//�ڴ�ط���
		static char *start_free;
		static char *end_free;
		static size_t heap_size;

		//�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes)
		{
			return ((bytes + _ALIGN - 1)&~(_ALIGN - 1));
		}

		//free-list����
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return ((bytes + _ALIGN - 1) / _ALIGN - 1);
		}

		//free-list��ĳ�������޿��ÿռ�ʱ�����ڴ�ػ�ȡ�ռ�
		static void *refill(size_t n);
		//���ڴ�ػ�ȡ���ÿռ�
		static char *chunk_alloc(size_t size, int &nobjs);

	public:
		static void *allocate(size_t n);
		static void deallocate(void *p, size_t n);
		static void *reallocate(void *p, size_t old_sz, size_t new_sz);
	};
}
#endif
