#include <stdio.h> 
#include <string.h>
#include <limits.h> //int때문에?

// 시간 전역 변수 
float CT[8] = { 0, };
float TAT[8] = { 0, };
float WT[8] = { 0, };

// 스레드 개수 전역 변수로 정의 
int threadnum = 0;

// SJF 비선점형
int SJF(char thread[], int arrtime[], int exetime[], int n) {
    int fin[100] = { 0 };
    int time = 0;

    for (int i = 0; i < n; i++) {
        int idx = -1;
        int min = 0;

        for (int j = 0; j < n; j++) {
            if (!fin[j] && arrtime[j] <= time && exetime[j] < min) {
                min = exetime[j];
                idx = j;
            }
        } //minimum burst time

        if (idx != -1) {
            time += exetime[idx];
            CT[idx] = time;
            TAT[idx] = CT[idx] - arrtime[idx];
            WT[idx] = TAT[idx] - exetime[idx];
            fin[idx] = 1;
        }
        else {
            //드르렁 
        }
    }

    return 0;
}

// SJF 선점형
int SJFpree() {
    int left[100];
    for (int i = 0; i < n; i++) {
        left[i] = exetime[i];
    } //초기화



    return 0;
}

int main() {
    // input: txt 입력받아서 처리 
    char* thread[100]; // 스레드 
    memset(thread, 0, sizeof(thread));

    int arrtime[100]; // 도착시간 
    memset(arrtime, 0, sizeof(arrtime));

    int exetime[100]; // 실행시간 
    memset(exetime, 0, sizeof(exetime));

    int pri[100]; // 우선순위(클수록 높음)
    memset(pri, 0, sizeof(pri));

    // 나 여기 인풋 만들겠음 
    int i = 0;
    do {
        scanf("%s %d %d %d", thread[i], &arrtime[i], &exetime[i], &pri[i]);
    } while (strcmp(thread[i], "E") != 0);

    threadnum = i;

    // 알고리즘 실행 
    FCFS(thread, arrtime, exetime);

    // 타임 출력 
    // TODO: 줄맞춤해야됨(\t였던듯) 
    printf("Results Completed Time Turnaround Time Waiting Time\n----------------------------------\n");
    for (int j = 0; j < threadnum; j++) {
        printf("%s %.2f %.2f %.2f\n", thread[j], CT[j], TAT[j], WT[j]);
    }
}
