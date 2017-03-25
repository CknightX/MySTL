#include "ck_alloc.h"

namespace CK_STL
{
	char *alloc::start_free = nullptr; //�ڴ�ؿ�ʼ��ַ
	char *alloc::end_free = nullptr;
	size_t alloc::heap_size = 0;
	alloc::obj *alloc::free_list[_NFREELISTS] = { nullptr };

	void* alloc::allocate(size_t n)
	{
		// ����128 ʹ��malloc����
		if (n > _MAX_BYTES)
			return malloc(n);

		//ʹ��alloc
		int index = FREELIST_INDEX(n);
		obj* result = free_list[index];

		if (result)
		{
			free_list[index] = result->next_free_list;
			return result;
		}
		//free-list��û��ʣ��ռ䣬���ڴ�����»�ȡ
		else
		{
			void* r = refill(ROUND_UP(n));
			return r;
		}
	}
	void alloc::deallocate(void* p, size_t n)
	{
		// ����128�黹��
		if (n > _MAX_BYTES)
		{
			free(p);
			return;
		}
		// С��128�黹free-list
		else
		{
			int index = FREELIST_INDEX(n);
			obj *q = static_cast<obj*>(p);
			q->next_free_list = free_list[index]; //����free_listͷ��
			free_list[index] = q;
		}
	}
	void* alloc::reallocate(void *p, size_t old_sz, size_t new_sz)
	{
		deallocate(p, old_sz);
		p = allocate(new_sz);
		return p;
	}
	void* alloc::refill(size_t n)
	{
		int nobjs = 20;
		char *chunk = chunk_alloc(n, nobjs);

		//ֻ��ȡ����һ����
		if (nobjs == 1)
		{
			return chunk; //ֱ�ӷ��ظ��Ͷ�
		}

		//��һ����Ͷˣ�����������free-list
		obj *result = (obj*)(chunk); //���Ͷ�
		obj *next_obj = (obj*)(chunk+n);  //���ý�free_list��ͷ
		obj *current_obj = (obj*)(next_obj);
		free_list[FREELIST_INDEX(n)] = current_obj; //����free_listָ�������õĿռ�

		for (int i = 1;; ++i)
		{
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + n);

			if (i == nobjs - 1)
			{
				current_obj->next_free_list = 0;
				break;
			}
			else
			{
				current_obj->next_free_list = next_obj;
			}
		}
		return result;
	}
	char* alloc::chunk_alloc(size_t size, int &nobjs)
	{
		char *result;
		size_t total_bytes = size*nobjs; //�����ܴ�С
		size_t bytes_left = end_free - start_free; //�ڴ��ʣ��ռ�

		//�ڴ��������Ҫ
		if (bytes_left >= total_bytes)
		{
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		//�ڴ��ʣ��ռ䲻����ȫ������Ҫ�����㹻��Ӧһ�����������ϵ�����
		else if (bytes_left >= size)
		{
			nobjs = bytes_left / size;
			total_bytes = size*nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		// �ڴ��ʣ��ռ�һ�����鶼�޷�����
		else
		{
			if (bytes_left > 0) //s
			{
				int index = FREELIST_INDEX(bytes_left);
				((obj*)(start_free))->next_free_list = free_list[index];
				free_list[index] = (obj*)(start_free);
			}
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			start_free = static_cast<char*>(malloc(bytes_to_get));

			if (start_free == 0) //����ʧ��
			{
				for (int i = size; i <= _MAX_BYTES; i += _ALIGN)
				{
					int index = FREELIST_INDEX(i);
					if (free_list[index] != 0)
					{
						start_free = (char*)(free_list[index]);
						free_list[index] = free_list[index]->next_free_list;
						end_free = start_free + i;
						return chunk_alloc(size, nobjs);
					}
				}
				end_free = 0;
				return nullptr;
			}
			else //��������ɹ�
			{
				heap_size += bytes_to_get;
				end_free = start_free + bytes_to_get;
				return chunk_alloc(size, nobjs);
			}
		}
	}
}
