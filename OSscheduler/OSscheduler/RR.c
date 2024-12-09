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

    int last = 0;
    int time = 0;   // 현재 시간
    int completed = 0; // 완료된 작업 수
    int prev_idx = -1; // 이전에 실행 중이던 스레드 인덱스

    // 각 스레드의 남은 실행 시간을 초기화
    for (int i = 0; i < n; i++) {
        left[i] = th[i].exetime;
    }

    int flag = 0;
    while (completed < n) {
        int idx = findpriority(n, time); // 현재 시간에 실행 가능한 우선순위 높은 스레드

        if (idx == -1) { // 실행 가능한 스레드가 없으면 시간 증가
            time++;
            continue;
        }

        // 스레드 변경 시 시작 시간 기록
        if (idx != prev_idx) {
            printf("%d : %s \n", time, th[idx].tid);
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
    printf("%d : #\n", time);


    return 0;
}
