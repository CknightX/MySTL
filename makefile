main : ck_alloc.o heap_test.o p_queue_test.o
	g++ -g -o main main.cpp ck_alloc.o TEST/heap_test.o  TEST/p_queue_test.o
ck_alloc.o :
	g++ -g -c ck_alloc.h ck_alloc.cpp
heap_test.o :
	g++ -g -c TEST/heap_test.cpp -o TEST/heap_test.o
p_queue_test.o :
	g++ -g -c TEST/p_queue_test.cpp -o TEST/p_queue_test.o
clean :
	rm ck_alloc.o TEST/heap_test.o main
