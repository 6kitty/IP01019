#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <limits.h> //int때문에?

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

// FCFS
int FCFS(int n) {
    printf("FCFS\n");
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

    printf("#");

    WT[0] /= n;
    TAT[0] /= n;
    CT[0] /= n;

    return 0;
}


int main() {
    int i = 0;
    char input[256];
    int threadnum = 0;
    while (fgets(input, sizeof(input), stdin)) {
        // 입력이 "E"이면 종료
        if (strcmp(input, "E\n") == 0 || strcmp(input, "E") == 0) {
            break;
        }

        // 공백을 기준으로 문자열 분리
        char* token = strtok(input, " ");
        if (token != NULL) {
            // 첫 번째 값: tid
            strncpy(th[i].tid, token, sizeof(th[i].tid) - 1);
            th[i].tid[sizeof(th[i].tid) - 1] = '\0'; // null-terminate

            // 두 번째 값: arrtime
            token = strtok(NULL, " ");
            if (token != NULL) {
                th[i].arrtime = atoi(token);
            }
            else {
                printf("Error: No data\n");
                continue;
            }

            // 세 번째 값: exetime
            token = strtok(NULL, " ");
            if (token != NULL) {
                th[i].exetime = atoi(token);
            }
            else {
                printf("Error: No data\n");
                continue;
            }

            // 네 번째 값: priority
            token = strtok(NULL, " ");
            if (token != NULL) {
                th[i].priority = atoi(token);
            }
            else {
                printf("Error: No data\n");
                continue;
            }

            i++; // 스레드 개수 증가

            // 배열 초과 방지
            if (i >= MAX) {
                printf("WARNING: overflow\n");
                break;
            }
        }
    }

    threadnum = i;

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
    printf("\n\nResults\t\tCompleted Time\tTurnaround Time\tWaiting Time\n");
    printf("------------------------------------------------------------------------\n");

    for (int j = 0; j < 8; j++) {
        printf("%-25s\t%.2f\t\t%.2f\t\t%.2f\n", algo[j], CT[j], TAT[j], WT[j]);
    }
}
