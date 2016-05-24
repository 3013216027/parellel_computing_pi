/* **********************************************

  File Name: pi.cpp

  Author: zhengdongjian@tju.edu.cn

  Created Time: Fri 20 May 2016 09:05:33 AM CST

*********************************************** */
#include <bits/stdc++.h>
using namespace std;

#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

const int MAX_THREAD_NUM = 64; //最大线程数
int N, threadNum, T; //N计算量, threadNum线程数, T执行次数
double pi = 0; //计算结果

pthread_t threads[MAX_THREAD_NUM]; //线程pid
int ids[MAX_THREAD_NUM]; //线程id(0->threadNum-1)
sem_t lock; //临界区互斥锁

void* gao(void* id) {
	int me = *(int*)id; //what's my id?
	double tmp, sum = 0.0; //temp sumary

	for (register int i = me; i <= N; i += threadNum) {
		//work with arrange_i
		tmp = (i + 0.5) / N;
		sum += 4.0 / (1 + tmp * tmp);
	}
	sum /= N;
	sem_wait(&lock);
	pi += sum;
	sem_post(&lock);
}

int main(int argc, char** argv) {
	if (argc < 4) {
		fprintf(stderr, "Usage: %s <N> <thread_num> <repeat time for calculating>\n", argv[0]);
		exit(1);
	}
	/* init */
	N = atoi(argv[1]);
	threadNum = std::min(atoi(argv[2]), MAX_THREAD_NUM);
	T = atoi(argv[3]);
	sem_init(&lock, 0, 1); //init semaphore, with initial value of 1

	struct timeval time_begin, time_end;
	gettimeofday(&time_begin, NULL); //start time

	/* repeat for T times */
	for (int i = 0; i < T; ++i) {
		pi = 0.0;
		for (int j = 0; j < threadNum; ++j) {
			ids[j] = j;
			pthread_create(threads + j, NULL, gao, (void*)(ids + j));
		}
		for (int j = 0; j < threadNum; ++j) {
			pthread_join(threads[j], NULL); //wait for each thread finish his work :)
		}
	}

	gettimeofday(&time_end, NULL); //end time

	double time_passed = (time_end.tv_sec - time_begin.tv_sec) * 1000.0 + (time_end.tv_usec - time_begin.tv_usec) / 1000.0; //ms
	double avg_time = time_passed / T;

	printf("get π = %.15f, avarage time used = %.6f ms\n", pi, avg_time);

	return 0;
}
