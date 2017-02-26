#ifndef _CK_VECTOR_H
#define _CK_VECTOR_H
#include "ck_allocator.h"
namespace CK_STL
{
	template < class T, class Alloc = allocator >
	class vector
	{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type* iterator;
		typedef value_type& reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
	protected:
		iterator start;
		iterator finish;
		iterator end_of_storage;
	public:
		//����״̬
		size_type size()const{ return static_cast<size_type>(end() - begin()); }
		size_type capacity()const{ return static_cast<size_type>(end_of_storage - begin()); }
		bool empty() const { return begin() == end(); }

		//Ԫ�ط���
		reference operator[](size_type n){ return *(begin() + n); }
		reference front(){ return *(begin()); }
		reference back(){ return *(end() - 1); }

		//������
		iterator begin(){ return start; }
		iterator end(){ return finish; }

		//���캯��
		vector() :start(nullptr), finish(nullptr), end_of_storage(nullptr){}
		vector(size_type n, const T& value){ fill_initialize(n, value); }
		vector(int n, const T& value){ fill_initialize(n, value); }
		vector(long n, const T& value){ fill_initialize(n, value); }
		explicit vector(size_type n){ fill_initialize(n, T()); } //explicit ��ֹ��ʽת��

		//������ɾ��
		void push_back(const T& x);
		void pop_back();
		void resize(size_type new_size, const T& x);
		void resize(size_type new_size){ resize(new_size, T()); }
		void clear(){ erase(begin(), end()); }
		iterator erase(iterator position);

		//��������
		~vector(){destroy(start, finish);}
	};
}
#endif