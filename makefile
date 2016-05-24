N=100000
thread_num=16
T=6

default: pi.cpp
	g++ -pthread -o pi pi.cpp

test: pi
	./pi ${N} ${thread_num} ${T}
