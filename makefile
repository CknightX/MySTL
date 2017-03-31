main : main.o ck_alloc.o
	g++ -o main main.o ck_alloc.o
main.o : main.cpp
	g++ -c main.cpp
ck_alloc.o : ck_alloc.h ck_alloc.cpp
	g++ -c ck_alloc.h ck_alloc.cpp
