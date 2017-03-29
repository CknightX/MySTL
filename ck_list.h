#ifndef _CK_LIST_H
#define _CK_LIST_H
#include "ck_iterator.h"
#include "ck_alloc.h"
#include "ck_simple_alloc.h"
#include "ck_functional.h"
namespace CK_STL
{
	template<class T>
	struct list_node
	{
		list_node* next;
		list_node* prev;
		T data;
	};

	template<class T>
	struct list_iterator:public iterator<bidirectional_iterator_tag,T>
	{
		typedef list_node<T>* link_type;
		typedef list_iterator self;
		link_type node; //迭代器内部指针，指向list节点

		//constructor
		list_iterator(link_type x) :node(x){}
		list_iterator(){}
		list_iterator(const self& x) :node(x.node){}

		bool operator==(const self& x){ return node == x.node; }
		bool operator!=(const self& x){ return node != x.node; }

		reference operator*(){ return (*node).data; }
		pointer operator->(){ return &(operator*());}

		self& operator++()
		{
			node = node->next;
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return tmp;
		}
		self& operator--()
		{
			node = node->prev;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			--*this;
			return tmp;
		}
	};
	template<class T,class Alloc=alloc>
	class list
	{
	protected:
		typedef list_node<T> list_node;
		typedef list_node* link_type;
		typedef simple_alloc<list_node,Alloc> node_allocator;
	public:
		typedef T value_type;
		typedef list_iterator<T> iterator;
		typedef list_iterator<T> const_iterator;


		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
	protected:
		list_node* node; 
	public:
		//constructor & destructor
		void init()
		{
			node = alloc_node();
			node->next = node->prev = node;
		}
		list()
		{
			init();
		}
		list(int n, const T& val)
		{
			init();
			while (n--)
				push_back(val);
		}
		template<class InputIterator>
		list(const list& l);
		list& operator=(const list& l){}
		~list()
		{
			clear();
			dealloc_node(node);
		}

		//iterator
		iterator begin(){ return node->next; }
		const_iterator begin()const{ return node->next; }
		iterator end(){ return node; }
		const_iterator end()const{ return node; }

		bool empty()const{ return node->next == node; }
		size_type size()const{ return CK_STL::distance(begin(), end()); }
		reference front(){ return *begin(); }
		reference back(){ return *(--end()); }
		void swap(list& x){ swap(node, x.node); }

		//node con & de & init
		list_node* alloc_node(){ return node_allocator::allocate(); }
		void dealloc_node(list_node* node){ node_allocator::deallocate(node); }
		list_node* create_node(const T& x){ link_type tmp = alloc_node(); tmp->next = tmp->prev = nullptr; tmp->data = x; return tmp; }
		void destroy_node(list_node* node){ destroy(&node->data); dealloc_node(node); }

		iterator insert(iterator position, const T& x)
		{
			link_type tmp = create_node(x);
			tmp->next = position.node;
			tmp->prev = position.node->prev;
			tmp->prev->next = tmp;
			position.node->prev = tmp;
			return iterator(tmp);
		}
		void insert(iterator position, int n, const T& x)
		{
			for (; n > 0; --n)
				position = insert(position, x);
		}
		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last)
		{
			for (; first != last; ++first)
				position = insert(position, *first);
		}

		void push_front(const T& x){ insert(begin(), x); }
		void push_back(const T& x){ insert(end(), x); }
		void pop_front(){ erase(begin()); }
		void pop_back(){ erase(--end()); }

		iterator erase(iterator position)
		{
			iterator next_node = position.node->next;
			iterator prev_node = position.node->prev;
			prev_node.node->next = next_node.node;
			next_node.node->prev = prev_node.node;
			destroy_node(position.node);
			return iterator(next_node);
		}
		iterator erase(iterator first, iterator last)
		{
			link_type cur = first.node; //head
			while (cur != last.node)
			{
				link_type tmp = cur;
				cur = cur->next;
				destroy_node(tmp);
			}
			return cur;
		}
		void clear()
		{
			erase(begin(), end());
			node->next = node->prev = node; //恢复node的状态
		}
		void remove(const T& value)
		{
			/*
			iterator iter = begin();
			iterator last = end();
			while (iter != last)
			{
			if (iter.node->data == value)
			iter = erase(iter);
			else
			++iter;
			}
			*/
			auto f = [value](const T& x)
			{
				return x == value; 
			};
			remove_if(f);
		}
		template<class Predicate>
		void remove_if(Predicate pred)
		{
			iterator iter = begin();
			iterator last = end();
			while (iter != last)
			{
				if (pred(iter.node->data))
					iter = erase(iter);
				else
					++iter;
			}
		}
		void unique()
		{
			iterator first = begin();
			iterator last = end();
			if (first == last)
				return;
			iterator next = first;
			while (++next != last)
			{
				if (*next == *first)
				{
					erase(next);
					next = first;
				}
				else
					first = next;
			}
		}

	protected:
		void transfer(iterator position, iterator first, iterator last)
		{
			if (position == last)
				return;
			position.node->prev->next = first.node;
			first.node->prev->next = last.node;
			last.node->prev->next = position.node;
			auto tmp = last.node->prev;
			last.node->prev = first.node->prev;
			first.node->prev = position.node->prev;
			position.node->prev = tmp;
		}
	public:
		void splice(iterator position, list& x){ if (!x.empty())transfer(position, x.begin(), x.end()); }
		void splice(iterator position, list&, iterator i)
		{
			iterator j = i;
			++j;
			if (position == i || position == j)
				return;
			transfer(position, i, j);
		}
		void splice(iterator position, list&, iterator first, iterator last){ if (first != last)transfer(position, first, last); }
		void merge(list& x)
		{
			merge(x, less<T>());
		}
		template<class Compare>
		void merge(list& x, Compare comp)
		{
			iterator first1 = begin();
			iterator last1 = end();
			iterator first2 = x.begin();
			iterator last2 = x.end();

			//两个list均递增
			while (first1 != last1&&first2 != last2)
			{
				if (comp(*first2, *first1))
				{
					iterator next = first2;
					transfer(first1, first2, ++next);
					first2 = next;
				}
				else
					++first1;
			}
			if (first2 != last2)
				transfer(last1, first2, last2);
		}
		void reverse()
		{
			if (node->next == node || node->next->next == node) //只有0或1个元素时不操作
				return;
			auto point = list_iterator<T>(node->next->next);
			while (point!=node)
			{
				auto tmp = point;
				++point;
				auto tmp2 = point;
				transfer(begin(), tmp, tmp2);
			}
		}
		void sort()
		{
			sort(less<T>());
		}
		template<class Compare>
		void sort(Compare comp) //comp 作为元素之间的比较方法
		{
			if (node->next == node || node->next->next == node) //只有0或1个元素时不操作
				return;

		}


		//operator
		bool operator==(const list& x)
		{
			const_iterator first1 = begin();
			const_iterator last1 = end();
			const_iterator first2 = x.begin();
			const_iterator last2 = x.end();
			while (first1 != last1&&first2 != last2&&*first1 == *first2)
			{
				++first1;
				++first2;
			}
			return first1 == last1&&first2 == last2;
		}
		bool operator!=(const list& x)
		{
			return !(*this == x);
		}
		bool operator<(const list& x)
		{
			return lexicographical_compare(begin(), end(), x.begin(), x.end());
		}
		bool operator>(const list& x)
		{
			return x < (*this);
		}
		bool operator<=(const list& x)
		{
			return !((*this) > x);
		}
		bool operator>=(const list& x)
		{
			return !((*this) < x);
		}
	};
}
#endif
