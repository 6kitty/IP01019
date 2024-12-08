#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// 시간 전역 변수
float CT[8] = { 0, };
float TAT[8] = { 0, };
float WT[8] = { 0, };

#define MAX 100 // 최대 스레드 수

// 스레드 정보를 담는 구조체
typedef struct {
	char tid[10];      // 스레드 ID
	int arrtime;       // 도착 시간
	int exetime;       // 실행 시간
	int priority;      // 우선순위
} Thread;

Thread th[MAX];

FILE* input = NULL;
FILE* output = NULL;

int LJF(int n) {
	fprintf(output, "LJF (Non-Preemptive)\n");
	int nn = 0;
	int time = 0;
	fprintf(output, "0 : ");
	while (nn < n) {
		int idx = -1;
		int max = -1;
		for (int i = 0; i < n; i++) {
			if (th[i].exetime <= 0) { continue; }
			if (th[i].arrtime <= time && th[i].exetime > max) {
				max = th[i].exetime;
				idx = i;
			}
		}
		if (idx == -1) {
			time++;
			continue;
		}
		if (th[idx].arrtime > time) {
			fprintf(output, "- (%d)\n%d : ", th[idx].arrtime, th[idx].arrtime + time);
			time = th[idx].arrtime;
		}
		fprintf(output, "%s (%d)\n%d : ", th[idx].tid, th[idx].exetime, time + th[idx].exetime);
		WT[3] += time - th[idx].arrtime;
		TAT[3] += time - th[idx].arrtime + th[idx].exetime;
		CT[3] += time + th[idx].exetime;
		time += th[idx].exetime;
		th[idx].exetime = 0;
		nn += 1;
	}
	fprintf(output, "#\n");
	WT[3] /= n;
	TAT[3] /= n;
	CT[3] /= n;
	return 0;
}

int LJFpree(int n) {
	fprintf(output, "LJF (Non-Preemptive)\n");
	int left[MAX];
	fprintf("0 : ");
	for (int i = 0; i < n; i++) {
		left[i] = th[i].exetime;
	}
	int time = 0;
	while (1) {
		int idx = -1;
		int max = -1;
		for (int j = 0; j < n; j++) {
			if (left[j] > 0 && th[j].arrtime <= time && left[j] > max) {
				max = left[j];
				idx = j;
			}
		}
		if (idx == -1) {
			int done = 1;
			for (int j = 0; j < n; j++) {
				if (left[j] > 0) {
					done = 0;
					break;
				}
			}
			if (done) break;
			time++;
			continue;
		}
		if (th[idx].arrtime > time) {
			printf("- (%d)\n%d : ", th[idx].arrtime, th[idx].arrtime + time);
			time = th[idx].arrtime;
		}
		left[idx]--;
		if (left[idx] == 0) {
			CT[4] += time + 1;
			TAT[4] += CT[4] - th[idx].arrtime;
			WT[4] += TAT[4] - th[idx].exetime;
		}
		time++;
	}
	CT[4] /= n;
	TAT[4] /= n;
	WT[4] /= n;
	return 0;
}