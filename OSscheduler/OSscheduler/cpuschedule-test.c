#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <cpuschedule_executable> <expected_output_file>\n", argv[0]);
        return 1;
    }

    char command[256];
    snprintf(command, sizeof(command), "%s < %s", argv[1], "thread.txt");  // cpuschedule 실행, thread.txt를 입력으로 사용
    char* output_file = "test_result.txt";
    FILE* output = fopen(output_file, "w");
    if (output == NULL) {
        printf("Error: Unable to open output file\n");
        return 1;
    }

    // cpuschedule 실행
    FILE* fp = popen(command, "r");
    if (fp == NULL) {
        printf("Error: Failed to run cpuschedule\n");
        fclose(output);
        return 1;
    }

    // cpuschedule 실행 결과를 읽어서 test_result.txt에 저장
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        fprintf(output, "%s", line);
    }

    fclose(output);
    fclose(fp);

    // 이제 thread_result.txt와 test_result.txt 비교
    FILE* cmp1 = fopen(argv[2], "r");
    FILE* cmp2 = fopen(output_file, "r");

    if (cmp1 == NULL || cmp2 == NULL) {
        printf("Error: Unable to open comparison files\n");
        return 1;
    }

    char buffer1[256], buffer2[256];

    const char* algo[] = {
        "FCFS", "SJF (non preemptive)", "SJF (preemptive)",
        "LJF (non preemptive)", "LJF (preemptive)", "priority(non preemptive)",
        "priority (preemptive)", "RR"
    };

    float compareCT[8] = { 0, };
    float compareTAT[8] = { 0, };
    float compareWT[8] = { 0, };
    float CT[8] = { 0, };
    float TAT[8] = { 0, };
    float WT[8] = { 0, };

    // 'Results'가 포함된 줄을 찾고, 그 줄부터 두 줄을 건너뜁니다.
    while (fgets(buffer1, sizeof(buffer1), cmp1)) {
        if (strstr(buffer1, "Results") != NULL) {
            fgets(buffer1, sizeof(buffer1), cmp1);  // 헤더라인(첫 번째 줄)을 건너뜁니다.
            break;
        }
    }
    while (fgets(buffer2, sizeof(buffer2), cmp2)) {
        if (strstr(buffer2, "Results") != NULL) {
            fgets(buffer2, sizeof(buffer2), cmp2);  // 헤더라인(첫 번째 줄)을 건너뜁니다.
            break;
        }
    }

    // test_result.txt와 thread_result.txt에서 각각의 결과를 읽어서 비교
    for (int i = 0; i < 8; i++) {
        fgets(buffer1, sizeof(buffer1), cmp1);
        char* token = strtok(buffer1, " \t\n");
        while (token != NULL && !isdigit(token[0]) && token[0] != '.') {
            token = strtok(NULL, " \t\n"); // 다음 토큰으로 이동
        }
        if (token != NULL) {
            CT[i] = atof(token);
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
            token = strtok(NULL, " \t\n");
        }
        if (token != NULL) {
            compareCT[i] = atof(token);
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

    // 결과 비교 출력
    cmp1 = fopen(argv[2], "r");
    cmp2 = fopen(output_file, "r");
    for (int i = 0; i < 8; i++) {
        printf("%s", algo[i]);
        printf(" : ");

        // 시뮬레이션 비교
        int flag = 0;
        while (1) {
            fgets(buffer1, sizeof(buffer1), cmp1);
            fgets(buffer2, sizeof(buffer2), cmp2);
            if (strcmp(buffer1, buffer2) != 0) {
                printf("FAIL, ");
                flag = 1;
                break;
            }
            if (strchr(buffer1, '#') != NULL) {
                break;
            }
        }
        if (flag == 0) {
            printf("PASS, ");
        }

        // Completed Time (CT) 비교
        if (CT[i] == compareCT[i]) {
            printf("PASS, ");
        }
        else {
            printf("FAIL, ");
        }

        // Turnaround Time (TAT) 비교
        if (TAT[i] == compareTAT[i]) {
            printf("PASS, ");
        }
        else {
            printf("FAIL, ");
        }

        // Waiting Time (WT) 비교
        if (WT[i] == compareWT[i]) {
            printf("PASS\n");
        }
        else {
            printf("FAIL\n");
        }
    }

    return 0;
}
