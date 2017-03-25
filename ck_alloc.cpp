#include "ck_alloc.h"

namespace CK_STL
{
	char *alloc::start_free = nullptr; //内存池开始地址
	char *alloc::end_free = nullptr;
	size_t alloc::heap_size = 0;
	alloc::obj *alloc::free_list[_NFREELISTS] = { nullptr };

	void* alloc::allocate(size_t n)
	{
		// 大于128 使用malloc分配
		if (n > _MAX_BYTES)
			return malloc(n);

		//使用alloc
		int index = FREELIST_INDEX(n);
		obj* result = free_list[index];

		if (result)
		{
			free_list[index] = result->next_free_list;
			return result;
		}
		//free-list中没有剩余空间，从内存池重新获取
		else
		{
			void* r = refill(ROUND_UP(n));
			return r;
		}
	}
	void alloc::deallocate(void* p, size_t n)
	{
		// 大于128归还堆
		if (n > _MAX_BYTES)
		{
			free(p);
			return;
		}
		// 小于128归还free-list
		else
		{
			int index = FREELIST_INDEX(n);
			obj *q = static_cast<obj*>(p);
			q->next_free_list = free_list[index]; //放在free_list头部
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

		//只获取到了一个块
		if (nobjs == 1)
		{
			return chunk; //直接返回给客端
		}

		//第一块给客端，其他串联进free-list
		obj *result = (obj*)(chunk); //给客端
		obj *next_obj = (obj*)(chunk+n);  //配置进free_list的头
		obj *current_obj = (obj*)(next_obj);
		free_list[FREELIST_INDEX(n)] = current_obj; //导引free_list指向新配置的空间

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
		size_t total_bytes = size*nobjs; //请求总大小
		size_t bytes_left = end_free - start_free; //内存池剩余空间

		//内存池满足需要
		if (bytes_left >= total_bytes)
		{
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		//内存池剩余空间不能完全满足需要，但足够供应一个（含）以上的区块
		else if (bytes_left >= size)
		{
			nobjs = bytes_left / size;
			total_bytes = size*nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		// 内存池剩余空间一个区块都无法分配
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

			if (start_free == 0) //分配失败
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
			else //重新申请成功
			{
				heap_size += bytes_to_get;
				end_free = start_free + bytes_to_get;
				return chunk_alloc(size, nobjs);
			}
		}
	}
}
