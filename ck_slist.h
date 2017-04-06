#ifndef _CK_SLIST_H
#define _CK_SLIST_H
#include "ck_iterator.h"
#include "ck_simple_alloc.h"
#include "ck_alloc.h"
#include "ck_construct.h"
#include <cstddef>
namespace CK_STL
{
class slist_node_base
{
	public:
		slist_node_base* next;
};

template<class T>
class slist_node:public slist_node_base
{
	public:
		T data;
};

inline slist_node_base* slist_make_link(slist_node_base* prev_node,slist_node_base* new_node)
{
	new_node->next=prev_node->next;
	prev_node->next=new_node;
	return new_node;
}

inline size_t slist_size(slist_node_base* node)
{
	size_t result=0;
	for(;node!=0;node=node->next)
		++result;
	return result;
}


class slist_iterator_base
{
	public:
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef forward_iterator_tag iterator_category;
	public:
			slist_iterator_base(slist_node_base* x):node(x){}

			void incr(){node=node->next;} //forward one node
			bool operator==(const slist_iterator_base& x)const{return node==x.node;}
			bool operator!=(const slist_iterator_base& x)const{return node!=x.node;}
	public:
			slist_node_base* node;
};

template<class T,class Ref,class Ptr>
struct slist_iterator:public slist_iterator_base
{
	typedef slist_iterator<T,T&,T*> iterator;
	typedef slist_iterator<T,const T&,const T*> const_iterator;
	typedef slist_iterator<T,Ref,Ptr> self;

	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef slist_node<T> list_node;

	slist_iterator(list_node* x):slist_iterator_base(x){}
	slist_iterator():slist_iterator_base(0){}
	slist_iterator(const iterator& x):slist_iterator_base(x.node){}

	reference operator*()const {return (static_cast<list_node*>(node))->data;}
	pointer operator->()const{return &(operator*());}

	self& operator++()
	{
		incr();
		return *this;
	}
	self operator++(int)
	{
		self tmp=*this;
		incr();
		return tmp;
	}
};

template<class T,class Alloc=alloc>
class slist
{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef slist_iterator<T,T&,T*> iterator;
		typedef slist_iterator<T,const T&,const T*> const_iterator;

	private:
		typedef slist_node<T> list_node;
		typedef slist_node_base list_node_base;
		typedef slist_iterator_base iterator_base;
		typedef simple_alloc<list_node,Alloc> list_node_allocator;
		static list_node* create_node(const value_type& x)
		{
			list_node* node=list_node_allocator::allocate();
			try
			{
				construct(&node->data,x);
				node->next=0;
			}
			catch(...)
			{
				list_node_allocator::deallocate(node);
			}
			return node;
		}

		static void destroy_node(list_node* node)
		{
			destroy(&node->data);
			list_node_allocator::deallocate(node);
		}
	private:
		list_node_base head;

	public:
		slist(){head.next=0;}
		~slist(){clear();}
	public:
		iterator begin(){return iterator(static_cast<list_node*>(head.next));}
		iterator end(){return iterator(static_cast<list_node*>(0));}
		size_type size()const{return slist_size(head.next);}
		bool empty()const{return head.next==0;}

		void swap(slist& L)
		{
			list_node_base* tmp=head.next;
			head.next=L.head.next;
			L.head.next=tmp;
		}
	public:
		reference front(){return (static_cast<list_node*>(head.next))->data;}
		void push_front(const value_type& x)
		{
			slist_make_link(&head,create_node(x));
		}
		void pop_front()
		{
			list_node* node=static_cast<list_node*>(head.next);
			head.next=node->next;
			destroy_node(node);
		}
		iterator insert(const iterator& iter,const value_type& x)
		{
			list_node_base* tmp=&head;
			for (;tmp->next!=iter.node;tmp=tmp->next);
			slist_make_link(tmp,create_node(x));

		}
		iterator erase(const iterator& iter)
		{
			list_node_base* tmp=&head;
			for (;tmp->next!=iter.node;tmp=tmp->next);
			list_node_base* tmp2=tmp->next;
			tmp->next=tmp2->next;
			destroy_node(static_cast<list_node*>(tmp2));
			return iterator(static_cast<list_node*>(tmp));
		}
		void clear()
		{
			list_node_base* tmp=head.next;
			while(tmp)
			{
				list_node_base* tmp2=tmp;
				tmp=tmp->next;
				destroy_node(static_cast<list_node*>(tmp2));
			}
		}
};
}
#endif
