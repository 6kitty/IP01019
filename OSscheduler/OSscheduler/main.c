int main() {
    // input: txt 입력받아서 처리 
    char* thread[100];
    memset(thread, 0, sizeof(thread));

    int arrtime[100];
    memset(arrtime, 0, sizeof(arrtime));

    int exetime[100];
    memset(exetime, 0, sizeof(exetime));

    int pri[100];
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