#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <limits.h>



int main(int argc, char* argv[]) {
    char command[256];
    strncpy(command,argv[1],sizeof(command)-1);
    command[sizeof(command)-1]='\0';
    char* output_file = "test_result.txt";
    FILE* output = fopen(output_file, "w");
    if (output == NULL) {
        printf("Error: No file\n");
        return 1;
    }
    int result = system(command);
    fclose(output);
    if (result != 0) {
        printf("Error: No executing command");
        return 1;
    }


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

    FILE* cmp1 = fopen(argv[2], "r");
    FILE* cmp2 = fopen("test_result.txt", "r");

    char buffer1[256],buffer2[256];

    //1. thread_result.txt 파싱하기 
    float compareCT[8] = { 0, };
    float compareTAT[8] = { 0, };
    float compareWT[8] = { 0, };
    float CT[8] = { 0, };
    float TAT[8] = { 0, };
    float WT[8] = { 0, };
    // 'Results'가 포함된 줄을 찾고, 그 줄부터 두 줄을 건너뜁니다.
    while (fgets(buffer1, sizeof(buffer1), cmp1)) {
        if (strstr(buffer1, "Results") != NULL) {
            // 'Results'가 포함된 줄을 찾으면,
            fgets(buffer1, sizeof(buffer1), cmp1);  // 헤더라인(첫 번째 줄)을 건너뜁니다.
            break;
        }
    }
    while (fgets(buffer2, sizeof(buffer2), cmp2)) {
        if (strstr(buffer2, "Results") != NULL) {
            // 'Results'가 포함된 줄을 찾으면,
            fgets(buffer2, sizeof(buffer2), cmp2);  // 헤더라인(첫 번째 줄)을 건너뜁니다.
            break;
        }
    }

    for (int i = 0; i < 8; i++) {
        fgets(buffer1, sizeof(buffer1), cmp1);
        char* token = strtok(buffer1, " \t\n");
        while (token != NULL && !isdigit(token[0]) && token[0] != '.') {
            token = strtok(NULL, " \t\n"); // 다음 토큰으로 이동
        }

        if (token != NULL) {
            CT[i] = atof(token);  // Completed Time (CT)
        }
        // Turnaround Time (TAT)
        token = strtok(NULL, " \t\n");
        if (token != NULL) {
            TAT[i] = atof(token);
        }

        // Waiting Time (WT)
        token = strtok(NULL, " \t\n");
        if (token != NULL) {
            WT[i] = atof(token);
        }
    }
    for (int i = 0; i < 8; i++) {
        fgets(buffer2, sizeof(buffer2), cmp2);
        char* token = strtok(buffer2, " \t\n");
        while (token != NULL && !isdigit(token[0]) && token[0] != '.') {
            token = strtok(NULL, " \t\n"); // 다음 토큰으로 이동
        }

        if (token != NULL) {
            compareCT[i] = atof(token);  // Completed Time (CT)
        }
        // Turnaround Time (TAT)
        token = strtok(NULL, " \t\n");
        if (token != NULL) {
            compareTAT[i] = atof(token);
        }

        // Waiting Time (WT)
        token = strtok(NULL, " \t\n");
        if (token != NULL) {
            compareWT[i] = atof(token);
        }
    }

    fclose(cmp1);
    fclose(cmp2);

    FILE* cmp1 = fopen(argv[2], "r");
    FILE* cmp2 = fopen("test_result.txt", "r");
    for (int i = 0; i < 8; i++) {
        printf(algo[i]);
        printf(" : ");

        //시뮬레이션 비교 
        int flag = 0;
        while (1) {
            fgets(buffer1, sizeof(buffer1), cmp1);
            fgets(buffer2, sizeof(buffer2), cmp2);
            if (strcmp(buffer1, buffer2) != 0) {
                printf("FAIL,");
                flag = 1;
                break;
            }
            if (strchr(buffer1, '#') != NULL) {
                break;
            }
        }
        if (flag == 0) {
            printf("PASS,");
        }

        //2. 비교하기 
        if (CT[i] == compareCT[i]) {
            printf("PASS,");
        }
        else {
            printf("FAIL,");
        }

        if (TAT[i] == compareTAT[i]) {
            printf("PASS,");
        }
        else {
            printf("FAIL,");
        }

        if (WT[i] == compareWT[i]) {
            printf("PASS\n");
        }
        else {
            printf("FAIL\n");
        }
    }
    fclose(cmp1);
    fclose(cmp2);

    return 0;
}
