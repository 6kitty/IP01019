#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <limits.h> //int때문에?
#define MAX 10


// 시간 전역 변수 
float CT[8] = { 0, };
float TAT[8] = { 0, };
float WT[8] = { 0, };

struct thread {
    char* tid;
    int arrtime;
    int exetime;
    int priority;
};

struct thread th[MAX];

// FCFS
int FCFS(int n) {
    int nn = 0;
    int time = 0;
    if (n == 0) {
        return -1;
    }

    printf("0 : ");

    while (nn < n) {
        int idx = -1;


        for (int i = 0; i < n; i++) {
            if (th[i].exetime<=0) { continue; }

            if (idx == -1 || th[i].arrtime < th[idx].arrtime) {
                idx = i;
            }
        }

        if (idx == -1)
            break;

        if (th[idx].arrtime > time) {
            printf("- (%d)\n%d : ", th[idx].arrtime, th[idx].arrtime + time);
            time += th[idx].arrtime;
        }
        printf("%s (%d)\n%d : ", th[idx].tid, th[idx].exetime, time + th[idx].exetime);

        WT[0] += time - th[idx].arrtime;
        TAT[0] += time - th[idx].arrtime + th[idx].exetime;
        CT[0] += time+th[idx].exetime;

        time += th[idx].exetime;
        th[idx].exetime = 0;
        nn += 1;
    }

    WT[0] /= n;
    TAT[0] /= n;
    CT[0] /= n;

    return 0;
}


int main() {
    int i = 0;
    char input[100];
    int threadnum = 0;
    while (1) {
        // 한 줄 입력받기
        fgets(input, sizeof(input), stdin);
        printf("Input: %s\n", input);

        // 종료 조건 확인
        if (input[0] == 'E' && strlen(input) == 2) { 
            break;
        }

        // 입력된 문자열을 구조체 멤버에 저장
        scanf(input, "%s %d %d %d", th[i].tid, &(th[i].arrtime), &(th[i].exetime), &(th[i].priority));
        i++;
    }

    threadnum = i+1;

    // 알고리즘 실행 
    FCFS(threadnum);

    // 타임 출력 
    const char* algo[] = {
    "FCFS",
    "SJF (non preemptive)",
    "SJF (preemptive)",
    "LJF (non preemptive)",
    "LJF (preemptive)",
    "priority(non preemptive)",
    "priority (preemptive)",
    "RR"
    };
    printf("\n\nResults\nAlgorithm\t\t\tCompleted Time\tTurnaround Time\tWaiting Time\n");
    printf("------------------------------------------------------------------------\n");

    for (int j = 0; j < 8; j++) {
        printf("%-25s\t%.2f\t\t%.2f\t\t%.2f\n", algo[j], CT[j], TAT[j], WT[j]);
    }
}
