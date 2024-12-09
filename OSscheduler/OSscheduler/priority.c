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

// FCFS
int FCFS(int n) {
    printf("\nFCFS\n");
    int nn = 0;
    int time = 0;
    if (n == 0) {
        return -1;
    }

    printf("0 : ");

    // 스레드 데이터를 복사하여 독립적인 실행을 보장
    Thread th_copy[MAX];
    memcpy(th_copy, th, sizeof(Thread) * n);

    while (nn < n) {
        int idx = -1;

        // 각 스레드 정보 수정 없이 원본을 유지
        for (int i = 0; i < n; i++) {
            if (th_copy[i].exetime <= 0) { continue; }

            if (idx == -1 || th_copy[i].arrtime < th_copy[idx].arrtime) {
                idx = i;
            }
        }

        if (idx == -1) break;

        if (th_copy[idx].arrtime > time) {
            printf("- (%d)\n%d : ", th_copy[idx].arrtime, th_copy[idx].arrtime + time);
            time += th_copy[idx].arrtime;
        }
        printf("%s (%d)\n%d : ", th_copy[idx].tid, th_copy[idx].exetime, time + th_copy[idx].exetime);

        WT[0] += time - th_copy[idx].arrtime;
        TAT[0] += time - th_copy[idx].arrtime + th_copy[idx].exetime;
        CT[0] += time + th_copy[idx].exetime;

        time += th_copy[idx].exetime;
        th_copy[idx].exetime = 0;  // 실행 후 복사본에서만 값 변경
        nn += 1;
    }

    printf("#\n");

    WT[0] /= n;
    TAT[0] /= n;
    CT[0] /= n;

    return 0;
}

// SJF (non-preemptive)
int SJF(int n) {
    printf("\nSJF (Non-Preemptive)\n");
    int nn = 0;
    int time = 0;

    printf("0 : ");

    // 스레드 데이터를 복사하여 독립적인 실행을 보장
    Thread th_copy[MAX];
    memcpy(th_copy, th, sizeof(Thread) * n);

    while (nn < n) {
        int idx = -1;
        int min = MAX;

        for (int i = 0; i < n; i++) {
            if (th_copy[i].exetime <= 0) { continue; }
            if (th_copy[i].arrtime <= time && th_copy[i].exetime < min) {
                min = th_copy[i].exetime;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        if (th_copy[idx].arrtime > time) {
            printf("- (%d)\n%d : ", th_copy[idx].arrtime, th_copy[idx].arrtime + time);
            time = th_copy[idx].arrtime;
        }

        printf("%s (%d)\n%d : ", th_copy[idx].tid, th_copy[idx].exetime, time + th_copy[idx].exetime);

        WT[1] += time - th_copy[idx].arrtime;
        TAT[1] += time - th_copy[idx].arrtime + th_copy[idx].exetime;
        CT[1] += time + th_copy[idx].exetime;

        time += th_copy[idx].exetime;
        th_copy[idx].exetime = 0;  // 실행 후 복사본에서만 값 변경
        nn += 1;
    }

    printf("#\n");

    WT[1] /= n;
    TAT[1] /= n;
    CT[1] /= n;

    return 0;
}


// SJF (preemptive)
int SJFpree(int n) {
    printf("\nSJF (Preemptive)\n");
    int left[MAX];
    Thread th_copy[MAX];
    memcpy(th_copy, th, sizeof(Thread) * n);

    for (int i = 0; i < n; i++) {
        left[i] = th_copy[i].exetime;
    }

    int time = 0;
    int completed = 0;
    int current_thread = -1;

    printf("0 : ");

    while (completed < n) {
        int idx = -1;
        int min = MAX;

        for (int j = 0; j < n; j++) {
            if (left[j] > 0 && th[j].arrtime <= time && left[j] < min) {
                min = left[j];
                idx = j;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        if (current_thread != idx) {
            printf("%s (%d)\n", th_copy[idx].tid, left[idx]);
            current_thread = idx;
        }

        int duration = left[idx];
        time += duration;
        left[idx] = 0;
        completed++;

        CT[2] += time;
        TAT[2] += time - th_copy[idx].arrtime;
        WT[2] += time - th_copy[idx].arrtime - th_copy[idx].exetime;

        if (completed < n) {
            printf("%d : ", time);
        }
    }
    printf("%d : #\n", time);

    CT[2] /= n;
    TAT[2] /= n;
    WT[2] /= n;

    return 0;
}


//LJF (non preemptive)
int LJF(int n) {
    printf("\nLJF (Non-Preemptive)\n");
    int nn = 0; // 완료된 스레드 개수
    int time = 0; // 현재 시간
    printf("0 : ");
    // 스레드 데이터를 복사하여 독립적인 실행을 보장
    Thread th_copy[MAX];
    memcpy(th_copy, th, sizeof(Thread) * n);
    while (nn < n) {
        int idx = -1;
        int max = -1;
        for (int i = 0; i < n; i++) {
            if (th_copy[i].exetime > 0 && th_copy[i].arrtime <= time && th_copy[i].exetime > max) {
                max = th_copy[i].exetime;
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        printf("%s (%d)\n%d : ", th_copy[idx].tid, th_copy[idx].exetime, time + th_copy[idx].exetime);
        WT[3] += time - th_copy[idx].arrtime;
        TAT[3] += time - th_copy[idx].arrtime + th_copy[idx].exetime;
        CT[3] += time + th_copy[idx].exetime;
        time += th_copy[idx].exetime;
        th_copy[idx].exetime = 0;  // 실행 후 복사본에서만 값 변경
        nn += 1;
    }
    printf("#\n");
    WT[3] /= n;
    TAT[3] /= n;
    CT[3] /= n;
    return 0;
}


int left[MAX];  // 남은 실행 시간

// LJF 기준으로 가장 긴 실행 시간을 가진 스레드 찾기
int findLongestJob(int n, int time) {
    int idx = -1;
    for (int i = 0; i < n; i++) {
        if (th[i].arrtime <= time && left[i] > 0) { // 실행 가능한 스레드만 고려
            if (idx == -1 || left[idx] < left[i]) {  // 남은 실행 시간이 더 긴 스레드 선택
                idx = i;
            }
        }
    }
    return idx;
}

// LJF (Preemptive)
int LJFpree(int n) {
    printf("\nLJF (Preemptive)\n");

    int time = 0;   // 현재 시간
    int completed = 0; // 완료된 작업 수
    int prev_idx = -1; // 이전에 실행 중이던 스레드 인덱스
    int prev_time = 0; // 이전 시간

    // 각 스레드의 남은 실행 시간을 초기화
    for (int i = 0; i < n; i++) {
        left[i] = th[i].exetime;
    }

    while (completed < n) {
        int idx = findLongestJob(n, time); // 현재 시간에 실행 가능한 남은 실행 시간이 가장 긴 스레드

        if (idx == -1) { // 실행 가능한 스레드가 없으면 시간 증가
            time++;
            continue;
        }

        // 스레드 변경 시 시작 시간과 실행시간 기록
        if (idx != prev_idx) {
            // 이전 시간과 차이를 계산해서 출력
            if (prev_idx != -1) {
                printf("%d : %s (%d) \n", prev_time, th[prev_idx].tid, time - prev_time); // 실행된 시간
            }
            prev_time = time;
            prev_idx = idx;
        }

        // 1단위 시간 실행
        left[idx]--;
        time++;

        // 스레드 완료 처리
        if (left[idx] == 0) {
            completed++;
        }
    }

    // 마지막으로 실행된 스레드 처리
    printf("%d : %s (%d) \n", prev_time, th[prev_idx].tid, time - prev_time);
    printf("%d : #\n", time);

    return 0;
}




// Priority (non-preemptive)
int PriorityNonPreemptive(int n) {
    printf("\nPriority (Non-preemptive)\n");
    int nn = 0;
    int time = 0;
    if (n == 0) {
        return -1;
    }

    printf("0 : ");

    Thread th_copy[MAX];
    memcpy(th_copy, th, sizeof(Thread) * n);

    while (nn < n) {
        int idx = -1;

        // 도착 시간이 같으면 우선순위로 비교
        for (int i = 0; i < n; i++) {
            if (th_copy[i].exetime > 0 && th_copy[i].arrtime <= time) {
                // 우선순위가 더 높은 작업을 찾음
                if (idx == -1 || th_copy[i].arrtime < th_copy[idx].arrtime || (th_copy[i].arrtime == th_copy[idx].arrtime && th_copy[i].priority < th_copy[idx].priority)) {
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            // 선택할 작업이 없으면 시간 증가
            int any_task_left = 0;
            for (int i = 0; i < n; i++) {
                if (th_copy[i].exetime > 0) {
                    any_task_left = 1;
                    break;
                }
            }
            if (any_task_left == 0) {
                break; // 더 이상 할 일이 없으면 종료
            }

            time++;  // 시간 증가
            continue; // 계속 루프
        }

        // 작업 실행
        printf("%s (%d)\n%d : ", th_copy[idx].tid, th_copy[idx].exetime, time + th_copy[idx].exetime);

        // 대기 시간, 반환 시간, 완료 시간 계산
        WT[6] += time - th_copy[idx].arrtime;
        TAT[6] += time - th_copy[idx].arrtime + th_copy[idx].exetime;
        CT[6] += time + th_copy[idx].exetime;

        time += th_copy[idx].exetime; // 현재 시간 갱신
        th_copy[idx].exetime = 0; // 작업 완료
        nn += 1;
    }

    printf("#\n");

    // 평균 계산
    WT[6] /= n;
    TAT[6] /= n;
    CT[6] /= n;

    return 0;
}


int left[MAX];  // 남은 실행 시간

int findpriority(int n, int time) {
    int idx = -1;
    for (int i = 0; i < n; i++) {
        if (th[i].arrtime <= time && left[i] > 0) { // 실행 가능한 스레드만 고려
            if (idx == -1 || th[idx].priority < th[i].priority) {
                idx = i;
            }
        }
    }
    return idx;
}

// Priority (Preemptive)
int PriorityPreemptive(int n) {
    printf("\nPriority (Preemptive)\n");

    int time = 0;   // 현재 시간
    int completed = 0; // 완료된 작업 수
    int prev_idx = -1; // 이전에 실행 중이던 스레드 인덱스
    int prev_time = 0; // 이전 시간

    // 각 스레드의 남은 실행 시간을 초기화
    for (int i = 0; i < n; i++) {
        left[i] = th[i].exetime;
    }

    while (completed < n) {
        int idx = findpriority(n, time); // 현재 시간에 실행 가능한 우선순위 높은 스레드

        if (idx == -1) { // 실행 가능한 스레드가 없으면 시간 증가
            time++;
            continue;
        }

        // 스레드 변경 시 시작 시간과 실행시간 기록
        if (idx != prev_idx) {
            // 이전 시간과 차이를 계산해서 출력
            if (prev_idx != -1) {
                printf("%d : %s (%d) \n", prev_time, th[prev_idx].tid, time - prev_time); // 실행된 시간
            }
            prev_time = time;
            prev_idx = idx;
        }

        // 1단위 시간 실행
        left[idx]--;
        time++;

        // 스레드 완료 처리
        if (left[idx] == 0) {
            completed++;
        }
    }

    // 마지막으로 실행된 스레드 처리
    printf("%d : %s (%d) \n", prev_time, th[prev_idx].tid, time - prev_time);
    printf("%d : #\n", time);

    return 0;
}




// RR
/*int RR(int n, int time_slice) {
    printf("RR \n");
    int left[MAX];

    printf("0 : ");

    for (int i = 0; i < n; i++) {
        left[i] = th[i].exetime;
    }

    int time = 0;
    while (1) {
        int done = 1;

        for (int i = 0; i < n; i++) {
            if (left[i] > 0) {
                done = 0;

                if (th[i].arrtime <= time) {
                    if (left[i] > time_slice) {
                        printf("%s (%d)\n%d : ", th[i].tid, time_slice, time + time_slice);
                        left[i] -= time_slice;
                        time += time_slice;
                    }
                    else {
                        printf("%s (%d)\n%d : ", th[i].tid, left[i], time + left[i]);
                        time += left[i];
                        left[i] = 0;
                    }
                }
            }
        }

        if (done) break;
        time++;
    }

    for (int i = 0; i < n; i++) {
        CT[3] += time;
        TAT[3] += CT[3] - th[i].arrtime;
        WT[3] += TAT[3] - th[i].exetime;
    }

    CT[3] /= n;
    TAT[3] /= n;
    WT[3] /= n;

    return 0;
}*/

int main() {
    int i = 0;
    char input[256];
    int threadnum = 0;
    int time_slice = 1; //일단 임의로 할당함

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
    SJF(threadnum);
    SJFpree(threadnum);
    LJF(threadnum);
    LJFpree(threadnum);
    PriorityNonPreemptive(threadnum);
    PriorityPreemptive(threadnum);
    //RR(threadnum, time_slice);

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
    printf("\n\nResults\t\t\tCompleted Time\tTurnaround Time\t\tWaiting Time\n");
    printf("-----------------------------------------------------------------------------\n");

    for (int j = 0; j < 8; j++) {
        printf("%-25s\t%.2f\t\t%.2f\t\t%.2f\n", algo[j], CT[j], TAT[j], WT[j]);
    }

    return 0;
}
