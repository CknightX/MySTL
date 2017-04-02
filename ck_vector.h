#ifndef _CK_VECTOR_H
#define _CK_VECTOR_H
#include "ck_alloc.h"
#include "ck_simple_alloc.h"
#include "ck_uninitialized.h"
#include "ck_algorithm.h"
#include "ck_construct.h"
#include<cstddef>
namespace CK_STL
{
	template < class T, class Alloc = alloc >
	class vector
	{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef vector<T, Alloc> self;
	protected:
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;
		iterator finish;
		iterator end_of_storage;
		void insert_aux(iterator position, const T& x)
		{
			if (finish != end_of_storage) //�б��ÿռ�
			{
				construct(finish, *(finish - 1));
				++finish;
				T x_copy = x;
				copy_backward(position, finish - 2, finish - 1);
				*position = x_copy;
			}
			else //�ޱ��ÿռ�
			{
				const size_type old_size = size();
				const size_type len = old_size != 0 ? 2 * old_size : 1;
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try
				{
					new_finish = uninitialized_copy(start, position, new_start);
					construct(new_finish, x);
					++new_finish;
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch (...)
				{
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				destroy(begin(), end()); //�������ͷ�ԭvector
				deallocate();

				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		}
		void deallocate()
		{
			if (start)
				data_allocator::deallocate(start, end_of_storage - start);
		}
		//��䲢��ʼ��
		void fill_initialize(size_type n, const T& value)
		{
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}
		//���ú���� 
		iterator allocate_and_fill(size_type n, const T& x)
		{
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n(result, n, x);
			return result;
		}

		template<class InputIterator>
		void allocate_and_copy(InputIterator first, InputIterator last)
		{
			start = data_allocator::allocate(last - first);
			finish = uninitialized_copy(first, last, start);
			end_of_storage = finish;
		}
		template<class InputIterator>
		iterator allocate_and_copy(size_type n, InputIterator first, InputIterator last)
		{
			iterator result = data_allocator::allocate(n);
			uninitialized_copy(first, last, result);
			return result;
		}
	public:

		//���캯��
		vector() :start(nullptr), finish(nullptr), end_of_storage(nullptr){}
		vector(size_type n, const T& value){ fill_initialize(n, value); }
		vector(int n, const T& value){ fill_initialize(n, value); }
		vector(long n, const T& value){ fill_initialize(n, value); }
		explicit vector(size_type n){ fill_initialize(n, T()); } //explicit ��ֹ��ʽת��

		//��������
		~vector()
		{
			destroy(start, finish);
			deallocate();
		}

		//����״̬
		size_type size()const{ return static_cast<size_type>(end() - begin()); }
		size_type capacity()const{ return static_cast<size_type>(end_of_storage - begin()); }
		bool empty() const { return begin() == end(); }

		//������ɾ��
		void push_back(const T& x)
		{
			if (finish != end_of_storage)
			{
				construct(finish, x);
				++finish;
			}
			else
				insert_aux(end(), x);
		}
		void pop_back()
		{
			if (!size())
				throw;
			--finish;
			destroy(finish);
		}
		void insert(iterator position, size_type n, const T& x) //��position������n��x
		{
			if (n != 0)
			{
				if (size_type(end_of_storage - finish) >= n) //���ÿռ��㹻
				{
					T x_copy = x;
					const size_type elems_after = finish - position; //�����֮���Ԫ�ظ���
					iterator old_finish = finish;
					if (elems_after > n) //�����֮���Ԫ�ظ�����������Ԫ�ظ���
					{
						uninitialized_copy(finish - n, finish, finish);
						finish += n;
						copy_backward(position, old_finish - n, old_finish);
						fill(position, position + n, x_copy);
					}
					else
					{
						uninitialized_fill_n(finish, n - elems_after, x_copy);
						finish += (n - elems_after);
						uninitialized_copy(position, old_finish, finish);
						finish += elems_after;
						fill(position, old_finish, x_copy);
					}
				}
				else //���ÿռ䲻��
				{
					const size_type old_size = size();
					const size_type new_size = old_size + max(old_size, n);
					iterator new_start = data_allocator::allocate(new_size);
					iterator new_finish = new_start;
					new_finish = uninitialized_copy(start, position, new_start);
					new_finish = uninitialized_fill_n(new_finish, n, x);
					new_finish = uninitialized_copy(position, finish, new_finish);

					destroy(start, finish);
					deallocate();
					start = new_start;
					finish = new_finish;
					end_of_storage = new_start + new_size;
				}
			}
		}
		iterator erase(iterator position) //����Ԫ��
		{
			if (position + 1 != end())
			{
				copy(position + 1, finish, position);
			}
			--finish; 
			destroy(finish); 
			return position;
		}
		iterator erase(iterator first, iterator last)
		{
			iterator i = copy(last, finish, start);
			destroy(i, finish);
			finish = finish-(last-first); // finish = i;
			return first;

		}
		//��������
		void reserve(size_type n) //����Ԥ���ռ�(������)
		{
			if (n < capacity()) 
			{

			}
			else
			{
				const size_type old_size = size();
				iterator tmp = allocate_and_copy(n, start, finish);
				destroy(start, finish);
				deallocate();
				start = tmp;
				finish = start + old_size;
				end_of_storage = start + n;
			}
		}
		void shrink_to_fit() //�Կռ䰴���Ż�
		{
			iterator new_start = data_allocator::allocate(size());
			finish = uninitialized_copy(start, finish, new_start);
			deallocate();
			start = new_start;
			end_of_storage = finish;
		}
		void resize(size_type new_size, const T& x)
		{
			if (new_size < size()) //���������
			{
				erase(begin() + new_size, end());
			}
			else
			{
				insert(end(), new_size - size(), x);
			}
		}
		void resize(size_type new_size){ resize(new_size, T()); }
		void clear(){ erase(begin(), end()); }

		//��ֵ
		self& operator=(const self& v)
		{
			if (this != &v)
			{
				const size_type len = v.size();
				if (len > capacity()) //��������
				{
					iterator tmp=allocate_and_copy(len,v.begin(), v.end());
					destroy(start, finish);
					deallocate();
					start = tmp;
					end_of_storage = start + len;
				}
				else if (size() >= len)
				{
					iterator i = copy(v.begin(), v.end(), begin());
					destroy(i, finish); //����Ĳ���
				}
				else
				{
					copy(v.begin(), v.begin() + size(), start);
					uninitialized_copy(v.begin() + size(), v.end(), finish);
				}
				finish = start + len;
			}
			return *this;
		}
		self& operator=(self&& v)
		{
			if (this != &v)
			{
				destroy(start, finish);
				deallocate();
				start = v.start;
				finish = v.finish;
				end_of_storage = v.end_of_storage;
				v.start = v.finish = v.end_of_storage = 0;
			}
			return *this;
		}
		//�����Ƚ�
		inline friend  bool operator==(const self& v1, const self& v2)
		{
			return (v1.size() == v2.size() && equal(v1.begin(), v1.end(), v2.begin()));
		}
		inline friend bool operator!=(const self& v1, const self& v2)
		{
			return !(v1 == v2);
		}
		inline friend bool operator<(const self& v1, const self& v2)
		{
			return lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end());
		}
		inline friend bool operator>(const self& v1, const self& v2)
		{
			return (v2<v1);
		}
		inline friend  bool operator<=(const self& v1, const self& v2)
		{
			return !(v1 > v2);
		}
		inline friend bool operator>=(const self& v1, const self& v2)
		{
			return !(v1 < v2);
		}
		//����
		void swap(self& v)
		{
			if (this != &v)
			{
				swap(start, v.start);
				swap(finish, v.finish);
				swap(end_of_storage, v.end_of_storage);
			}
		}

		//Ԫ�ط���
		reference operator[](const difference_type n){ return *(begin() + n); }
		const_reference operator[](const difference_type n)const{return *(cbegin() + n); }
		reference front(){ return *begin(); }
		reference back(){ return *(end() - 1); }
		pointer data(){ return start; }

		//������
		iterator begin(){ return start; }
		iterator end(){ return finish; }
		const_iterator begin()const{ return start; } //ʹconst����Ҳ�ܻ�ȡstart finish
		const_iterator end()const{ return finish; }
		const_iterator cbegin()const{ return start; }
		const_iterator ceng()const{ return finish; }

	};
}
#endif
