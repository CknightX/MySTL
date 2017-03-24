#ifndef _CK_DEQUE_H
#define _CK_DEQUE_H
#include "ck_alloc.h"
#include "ck_simple_alloc.h"
#include "ck_construct.h"
#include "ck_uninitialized.h"
#include "ck_algorithm.h"
namespace CK_STL
{
	inline size_t deque_buf_size(size_t n, size_t sz)
	{
		return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
	}
	template<class T, class Ref, class Ptr, size_t BufSize>
	struct deque_iterator
	{
		typedef deque_iterator<T, T&, T*, BufSize> iterator;
		typedef deque_iterator<T, const T&, const T*, BufSize> const_iterator;
		static size_t  buffer_size(){ return deque_buf_size(BufSize, sizeof(T)); }

		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T** map_pointer;
		typedef deque_iterator self;

		value_type* cur;
		value_type* first;
		value_type* last;
		map_pointer node;
		deque_iterator() :cur(nullptr), first(nullptr), last(nullptr), node(nullptr){}
		deque_iterator(value_type* x, map_pointer y) :cur(x), first(*y), last(*y + deque_buf_size()), node(y){}
		deque_iterator(const iterator& x) :cur(x.cur), first(x.first), last(x.last), node(x.node){}

		reference operator*(){ return *cur; }
		pointer operator->(){ return cur; }
		difference_type operator- (const deque_iterator& x)const{ return difference_type((buffer_size())*(node - x.node - 1) + (cur - first) + (x.last - x.cur)); }
		void set_node(map_pointer new_node)
		{
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}
		self& operator++()
		{
			++cur;
			if (cur == last)
			{
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return tmp;
		}
		self operator+(difference_type i)
		{
			self tmp = *this;
			for (; i > 0; --i)
				++tmp;
			return tmp;
		}
		self& operator--()
		{
			if (cur == first)
			{
				set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}
		self& operator--(int)
		{
			self tmp = *this;
			--*this;
			return tmp;
		}
		self& operator+=(difference_type n)
		{
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size()))
			{
				cur += n;
			}
			else
			{
				difference_type node_offset = offset>0 ? offset / difference_type(buffer_size()) : -difference_type((-offset - 1) / buffer_size()) - 1;
				set_node(node + node_offset);
				cur = first + (offset - node_offset*difference_type(buffer_size()));
			}
			return *this;
		}
		self operator+(difference_type n)const
		{
			self tmp = *this;
			return tmp += n;
		}

		self& operator-=(difference_type n)
		{
			return *this += (-n);
		}
		self operator- (difference_type n)const
		{
			self tmp = *this;
			return tmp -= n;
		}
		reference operator[](difference_type n)const{ return *(*this + n); }
		bool operator==(const self& x)const{ return cur == x.cur; }
		bool operator!=(const self& x)const{ return !(*this == x); }
		bool operator<(const self& x)const{ return (node == x.node) ? (cur < x.cur) : (node < x.node); }
	};

	template<class T, class Alloc = alloc, size_t BufSize = 0>
	class deque
	{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef deque_iterator < T, T&, T*, BufSize > _deque_iterator;
		typedef typename _deque_iterator::iterator iterator;
		typedef typename _deque_iterator::const_iterator const_iterator;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef deque<T, alloc, BufSize> self;
	protected:
		typedef pointer* map_pointer;
		typedef simple_alloc<value_type, alloc> data_allocator;
		typedef simple_alloc<pointer, alloc> map_allocator;
		iterator start;  //��һ���ڵ�
		iterator finish; //���һ���ڵ�
		map_pointer map; //ָ��map��mapΪ�洢ָ��Ŀռ�
		size_type map_size; //map���ж���ָ��
	public:
		//constructor
		deque(int n, const value_type& value) :start(), finish(), map(nullptr), map_size(0){ fill_initialize(n, value); }
		deque() :start(), finish(), map(nullptr), map_size(0){ create_map_and_nodes(0); }
		~deque(){}
	protected:
		enum { initial_map_size = 8 };
		static size_t buffer_size() { return deque_buf_size(0,sizeof(T)); }

		void fill_initialize(size_type n, const value_type& value);

		void reallocate_map(size_type nodes_to_add, bool add_at_front);
		void reserve_map_at_back(size_type nodes_to_add = 1);
		void reserve_map_at_front(size_type nodes_to_add = 1);
		void create_map_and_nodes(size_type num_elements);
		pointer allocate_node(){ return data_allocator::allocate(deque_buf_size(0,sizeof(T))); }
		void deallocate_node(value_type* p){ data_allocator::deallocate(p, deque_buf_size(0,sizeof(T))); }

		void push_back_aux(const value_type& v);
		void push_front_aux(const value_type& v);
		void pop_back_aux();
		void pop_front_aux();

		iterator insert_aux(iterator position, const value_type& x);
	public://�ⲿ�ӿ�
		iterator begin(){ return start; }
		iterator end(){ return finish; }

		reference operator[](size_type n){ return start[difference_type(n)]; }
		reference front(){ return *start; }
		reference back(){ return *(finish - 1); }

		size_type size()const{ return finish - start; }
		size_type max_size()const{ return size_type(-1); }
		bool empty()const{ return finish == start; }

		void push_back(const value_type& t);
		void push_front(const value_type& t);
		iterator insert(iterator position, const value_type& x); // !

		void clear();//!
		void pop_back();
		void pop_front();
		iterator erase(iterator pos); 
		iterator erase(iterator first, iterator last);//!

		void swap(self& x); 
		self& operator=(const self& x); //deep copy
	private:

	};

	
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	typename deque<T, Alloc, BufSize>::iterator deque<T, Alloc, BufSize>::erase(iterator first, iterator last)
	{
		if (first == start&&last == finish) //��������������deque��ֱ�ӵ���clear
		{
			clear();
			return finish;
		}
		else
		{
			difference_type n = last - first; //�������䳤��
			difference_type elems_before = first - start; //��������ǰ����Ԫ�ظ���
			if (elems_before < (size() - n) / 2) //ǰ��Ԫ�ؽ���
			{
				copy_backward(start, first, last);
				iterator new_start = start + n;
				destroy(start, new_start);
				for (map_pointer cur = start.node; cur < new_start.node; ++cur)
				{
					data_allocator::deallocate(*cur, buffer_size());
				}
				start = new_start;
			}
			else
			{
				copy(last, finish, first);
				iterator new_finish = finish - n;
				destroy(new_finish, finish);

				for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
				{
					data_allocator::deallocate(*cur, buffer_size());
				}
				finish = new_finish;
			}
			return start + elems_before;
		}
	}
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	typename deque<T, Alloc, BufSize>::iterator deque<T, Alloc, BufSize>::insert_aux(iterator position, const value_type& x)
	{
		difference_type index = pos - start; //�����֮ǰ��Ԫ�ظ���
		value_type x_copy = x;
		if (index < size() / 2)
		{
			push_front(front());  //��ǰ�˲���һ�����һ��Ԫ�صĿ���
			//		copy(start + 2, pos, start + 1);
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			copy(front2, pos1, front1);
		}
		else
		{
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;
			copy_backward(pos, back2, back1);
		}
		*pos = x_copy;
		return pos;
	}
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	typename deque<T, Alloc, BufSize>::iterator deque<T, Alloc, BufSize>::insert(iterator position, const value_type& x)
	{
		if (position.cur == start.cur) //���������ǰ
		{
			push_front(x);
			return start;
		}
		else if (position.cur == finish.cur)
		{
			push_back(x);
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else
		{
			return insert_aux(position, x);
		}
	}
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	typename deque<T, Alloc, BufSize>::iterator deque<T, Alloc, BufSize>::erase(iterator pos)
	{
		iterator next = pos;
		++next;
		difference_type index = pos - start;
		if (index < (size() >> 1)) //�����֮ǰ��Ԫ�رȽ���
		{
			copy_backward(start, pos, next); //�ƶ�֮ǰ��Ԫ��
			pop_front();  //ɾ����ǰ������Ԫ��
		}
		else
		{
			copy(next, finish, pos);
			pop_back();
		}
		return start + index;
	}
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::swap(self& x)
	{
		swap(map, x.map);
		swap(start, x.start);
		swap(finish, x.finish);
	}

	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::pop_front()
	{
		if (start.cur != start.last - 1) //��һ������������������Ԫ��
		{
			destroy(start.cur);
			++start.cur;
		}
		else
		{
			pop_front_aux();
		}
	}
	template<class T, class Alloc = alloc, size_t BufSize = 0>
	void deque<T, Alloc, BufSize>::pop_back()
	{
		if (finish.cur != finish.first)
		{
			--finish.cur;
			destroy(finish.cur);
		}
		else
		{
			pop_back_aux(); //�ͷŻ�����
		}
	}
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::push_front(const value_type& t)
	{
		if (start.cur != start.first) //��ǰnode���б��ÿռ�
		{
			--start.cur;
			construct(start.cur, t);
		}
		else //��ǰ�������ޱ��ÿռ�
		{
			push_front_aux(t);
		}
	}
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::push_back(const value_type& t)
	{
		if (finish.cur != finish.last - 1) //��󻺳�������2�����ϵ�Ԫ�ر��ÿռ�
		{
			construct(finish.cur, t);
			++finish.cur;
		}
		else //ֻ��һ��Ԫ�ر��ÿռ�
			push_back_aux(t);
	}

	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::fill_initialize(size_type n, const value_type& value)
	{
		create_map_and_nodes(n); //�����deque
		map_pointer cur;
		try
		{
			for (cur = start.node, cur < finish.node; ++cur)
			{
				uninitialized_fill(*cur, *cur + buffer_size(), value);
			}
			//���һ���ڵ㣺β�˿����б��ÿռ䣬�������ʼֵ
			uninitialized_fill(finish.first, finish.cur, value);
		}
		catch (...)
		{
			// pass
		}
	}

	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements)
	{
		size_type num_nodes = num_elements / buffer_size() + 1; //����Ҫ�ڵ���,�������������һ��(��ʼ����£�����һ���ڵ�)
		map_size = max(static_cast<size_type>(initial_map_size), num_nodes + 2);//һ��map������Ľڵ㣬����8�������Ϊ����ڵ�+2��Ϊ���䱸�ã�
		map = map_allocator::allocate(map_size);

		map_pointer nstart = map + (map_size - num_nodes) / 2; //���������룬ʹͷβ������ͬ����չ�ռ䡣
		map_pointer nfinish = nstart + num_nodes - 1;
		map_pointer cur;
		try
		{
			for (cur = nstart; cur <= nfinish; ++cur)
			{
				*cur = allocate_node();
			}
		}
		catch (...)
		{
			// commit or rollback
		}

		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + num_elements%buffer_size(); //�պ�����ʱ�������һ���ڵ㡣��ʱ��curָ���������һ���ڵ����ʼ��
	}
	//��map�Ĳ���
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::reserve_map_at_back(size_type nodes_to_add = 1) //β���ޱ��ÿռ�ʱ��map�Ĳ���
	{
		if (nodes_to_add > map_size - (finish.node - map + 1)) //���ÿռ�<����ռ�
		{
			reallocate_map(nodes_to_add, false);
		}
	}
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::reserve_map_at_front(size_type nodes_to_add = 1) //ͷ���ޱ��ÿռ�ʱ��map�Ĳ���
	{
		if (nodes_to_add > start.node - map) //���ÿռ�<����ռ�
		{
			reallocate_map(nodes_to_add, false);
		}
	}
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front)
	{
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;

		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes) //ʹ�õĿռ�ʵ����ԭmap��һ��
		{
			new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0); //δ���·���map��ֻ�ı�start
			if (new_nstart < start.node)
				copy(start.node, finish.node + 1, new_nstart); //�ص����֣�����
			else
				copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
		}
		else
		{
			size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2; //����һ���¿ռ�
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0); //��startָ����ȷ��λ��
			copy(start.node, finish.node + 1, new_nstart);
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}

	// push/pop aux
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::push_front_aux(const value_type& t)
	{
		value_type t_copy = t;
		reserve_map_at_front();
		*(start.node - 1) = allocate_node();
		try
		{
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			construct(start.cur, t_copy);
		}
		catch (...)
		{
			start.set_node(start.node + 1); //start�ص�ԭλ
			start.cur = start.first;
			deallocate_node(*(start.node - 1));
			throw;
		}
	}

	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::push_back_aux(const value_type& t)
	{
		value_type t_copy =t;
		reserve_map_at_back();	 //��map�޿ռ����,���ػ�һ��map
		*(finish.node + 1) = allocate_node();  //����һ���½ڵ�
		try
		{
			construct(finish.cur, t_copy);
			finish.set_node(finish.node + 1); //���һ��Ԫ�ر���䣬��finish�ƶ�����һ��node
			finish.cur = finish.first;
		}
		catch (...)
		{
			deallocate_node(*(finish.node + 1)); //Ԫ�ز���ʧ�ܣ����·����node deallocate
		}
	}
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::pop_front_aux()
	{
		destroy(start.cur);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}
	template<class T, class Alloc = alloc, size_t BufSize = 0 >
	void deque<T, Alloc, BufSize>::pop_back_aux()
	{
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		deallocate_node(finish.first);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}
}
#endif
