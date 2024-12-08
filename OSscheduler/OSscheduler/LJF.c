#include <stdio.h> 
#include <string.h>
#include <limits.h> //int때문에?

// 시간 전역 변수 
float CT[8] = { 0, };
float TAT[8] = { 0, };
float WT[8] = { 0, };

// 스레드 개수 전역 변수로 정의 
int threadnum = 0;

int LJF(char thread[], int arrtime[], int exetime[], int n) {
	int i, time = 0;
	int finish[100] = { 0 };
	printf("LJF (non preemptive)\n");
	printf("0 : ");
	while (i < n) {
		int idx = -1;
		int max = -1;
		for (i = 0; i < n; i++) {
			if(!finish)
		}
	}
}

int LJFpree() {

}