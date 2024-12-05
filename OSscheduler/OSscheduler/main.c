#include <stdio.h> 
#include <string.h>
#include <limits.h> //int때문에?

// 시간 전역 변수 
float CT[8] = { 0, };
float TAT[8] = { 0, };
float WT[8] = { 0, };

// 스레드 개수 전역 변수로 정의 
int threadnum = 0;

// FCFS
int FCFS(char* tmp[], int arrtime[], int exetime[]) {
    int n = threadnum;
    int nn = 0;
    if (threadnum == 0) {
        return -1;
    }
    int waittime[n];
    int tatime[n];
    int comtime[n];

    while (nn < n) {
        int idx = -1;
        // 파업합니다
        // 사유:  똥마려움
        // ? ㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋㅋ
    }
}

// SJF 비선점형
int SJF(char* thread[], int arrtime[], int exetime[], int n) {
    int time = 0; // 초기화

    return 0;
}

// SJF 선점형
int SJFpree() {

    return 0;
}

// LJF 비선점형
int LJF() {

    return 0;
}

// LJF 선점형
int LJFpree() {

    return 0;
}

// Priority 비선점형
int pri(char* thread[], int arrtime[], int exetime[], int pri[], int n) {
    int visited[100] = { 0 };
    int currentTime = 0;

    printf("Priority (non preemptive)\n");

    for (int count = 0; count < n; count++) {
        int idx = -1;
        int highestPriority = -1;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && arrtime[i] <= currentTime) {
                if (pri[i] > highestPriority) {
                    highestPriority = pri[i];
                    idx = i;
                }
            }
        }

        // 비선점형이니까 그냥 오는대로 시간 가게
        if (idx == -1) {
            currentTime++;
            count--;
            continue;
        }

        // 선택된 스레드 실행
        visited[idx] = 1;
        currentTime += exetime[idx];
        CT[idx] = currentTime;
        TAT[idx] = CT[idx] - arrtime[idx];
        WT[idx] = TAT[idx] - exetime[idx];

        printf("%d: %s(%d)\n", (int)(CT[idx] - exetime[idx]), thread[idx], exetime[idx]);
    }
    printf("%d: #\n", currentTime);
    return 0;
}

// Priority 선점형
int pripree(char* thread[], int arrtime[], int exetime[], int pri[], int n) {
    int remainingTime[100];
    int currentTime = 0;
    int completed = 0;

    for (int i = 0; i < n; i++) {
        remainingTime[i] = exetime[i];
    }
    printf("Priority (preemptive)\n");

    while (completed < n) {
        int idx = -1;
        int highestPriority = -1;

        // 도착한 것 중에서 우선순위 높은 거...
        for (int i = 0; i < n; i++) {
            if (arrtime[i] <= currentTime && remainingTime[i] > 0) {
                if (pri[i] < highestPriority) {
                    highestPriority = pri[i];
                    idx = i;
                }
            }
        }

        // 실행할 수 있는 거 없는 경우 그냥 시간 진행
        if (idx == -1) {
            currentTime++;
            continue;
        }

        // 선택한 거 실행
        printf("%d: %s(1)\n", currentTime, thread[idx]);
        remainingTime[idx]--;
        currentTime++;

        if (remainingTime[idx] == 0) {
            completed++;
            CT[idx] = currentTime;
            TAT[idx] = CT[idx] - arrtime[idx];
            WT[idx] = TAT[idx] - exetime[idx];
        }
    }
    printf("%d: #\n", currentTime);
    return 0;
}

// RR
int RR() {

    return 0;
}

int main() {
    int i = -1;
    char input[100];
    while (1) {
        // 한 줄 입력받기
        fgets(input, sizeof(input), stdin);

        // 종료 조건 확인
        if (input[0] == 'E' && strlen(input) == 2) { // "E" + 개행문자
            break;
        }

        // 입력된 문자열을 구조체 멤버에 저장
        sscanf(input, "%s %d %d %d", th[i].tid, &th[i].arrtime, &th[i].exetime, &th[i].priority);
        i++;
    }

    threadnum = i - 1;

    // 알고리즘 실행 
    FCFS();

    // 타임 출력 
    // TODO: 줄맞춤해야됨(\t였던듯) 
    printf("Results Completed Time Turnaround Time Waiting Time\n----------------------------------\n");
    for (int j = 0; j < threadnum; j++) {
        printf("%s %.2f %.2f %.2f\n", th[j].tid, CT[j], TAT[j], WT[j]);
    }
}
