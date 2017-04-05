#include "ck_heap.h"
#include "ck_vector.h"
#include "ck_functional.h"
using namespace CK_STL;
int main()
{
	int ia[5]={1,2,3,4,5};
	vector<int> a(ia,ia+5);
	heap_construct::make_heap(a.begin(),a.end(),less<int>());
}
