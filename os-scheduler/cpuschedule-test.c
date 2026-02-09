#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

// trim 함수: 문자열의 모든 공백 및 줄바꿈 문자 제거
void trim(char* str) {
    char* p = str;
    int i = 0;
    while (*p) {
        if (!isspace((unsigned char)*p)) {
            str[i++] = *p;
        }
        p++;
    }
    str[i] = '\0';
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <cpuschedule_executable> <expected_output_file>\n", argv[0]);
        return 1;
    }

    char command[256];
    snprintf(command, sizeof(command), "./%s", argv[1]);   // cpuschedule 실행
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

    for (int i = 0; i < 8; i++) {
        printf("%s", algo[i]);
        printf(" : ");

        int flag = 0;

        while (1) {
            if (fgets(buffer1, sizeof(buffer1), cmp1) == NULL || fgets(buffer2, sizeof(buffer2), cmp2) == NULL) {
                break;
            }

            if (strchr(buffer1, '#') != NULL || strchr(buffer2, '#') != NULL) {
                break;
            }

            trim(buffer1);
            trim(buffer2);

            if (strcmp(buffer1, buffer2) != 0) {
                printf("FAIL, ");
                flag = 1;
                break;
            }
        }

        if (flag == 0) {
            printf("PASS, ");
        }

        if (CT[i] == compareCT[i]) {
            printf("PASS, ");
        }
        else {
            printf("FAIL, ");
        }

        if (TAT[i] == compareTAT[i]) {
            printf("PASS, ");
        }
        else {
            printf("FAIL, ");
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
