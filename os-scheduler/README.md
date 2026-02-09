# OS CPU Scheduler Simulator

운영체제 CPU 스케줄링 알고리즘을 시뮬레이션하는 C 프로그램입니다. 8가지 스케줄링 알고리즘을 구현하고, 각 알고리즘의 성능 지표(Completion Time, Turnaround Time, Waiting Time)를 비교합니다.

## 구현된 알고리즘

| 알고리즘 | 설명 |
|---------|------|
| **FCFS** (First Come First Served) | 도착 순서대로 처리하는 비선점 스케줄링 |
| **SJF** (Shortest Job First, Non-Preemptive) | 실행 시간이 가장 짧은 작업을 우선 처리 |
| **SJF** (Shortest Job First, Preemptive) | 남은 실행 시간이 가장 짧은 작업을 우선 처리 (SRTF) |
| **LJF** (Longest Job First, Non-Preemptive) | 실행 시간이 가장 긴 작업을 우선 처리 |
| **LJF** (Longest Job First, Preemptive) | 남은 실행 시간이 가장 긴 작업을 우선 처리 |
| **Priority** (Non-Preemptive) | 우선순위가 높은 작업을 우선 처리 |
| **Priority** (Preemptive) | 더 높은 우선순위의 작업이 도착하면 선점 |
| **RR** (Round Robin) | 타임 슬라이스 단위로 순환하며 처리 |

## 빌드 및 실행

```bash
gcc -o scheduler main.c
./scheduler
```

## 입력 형식

표준 입력(stdin)으로 스레드 정보를 한 줄씩 입력합니다. 입력 종료 시 `E`를 입력합니다.

```
<스레드ID> <도착시간> <실행시간> <우선순위>
```

### 입력 예시

```
T1 0 5 2
T2 1 3 1
T3 2 8 3
T4 3 6 4
E
```

- **스레드ID**: 스레드 식별자 (최대 9자)
- **도착시간**: 스레드가 도착하는 시간 (정수)
- **실행시간**: 스레드의 CPU 실행 시간 (정수)
- **우선순위**: 스레드의 우선순위 (정수, 값이 클수록 높은 우선순위)

## 출력

각 알고리즘별로 스케줄링 간트 차트가 출력된 후, 모든 알고리즘의 평균 성능 지표가 표로 출력됩니다.

### 성능 지표

- **Completed Time (CT)**: 평균 완료 시간
- **Turnaround Time (TAT)**: 평균 반환 시간 (완료 시간 - 도착 시간)
- **Waiting Time (WT)**: 평균 대기 시간 (반환 시간 - 실행 시간)

### 출력 예시

```
FCFS
0 : T1 (5)
5 : T2 (3)
8 : T3 (8)
16 : T4 (6)
22 : #

...

Results                 Completed Time  Turnaround Time         Waiting Time
-----------------------------------------------------------------------------
FCFS                      ...             ...             ...
SJF (non preemptive)      ...             ...             ...
...
```

## 제약 사항

- 최대 스레드 수: 100개
- Round Robin 타임 슬라이스: 1 (코드 내 고정값)

## 6kitty 구현 내역

| 커밋 | 설명 |
|------|------|
| `5e51354` main | 프로젝트 초기 구조 작성 — `main()` 함수, 입력 파싱, FCFS 호출, 결과 출력 틀 구현 |
| `2d44853` Update main.c | main 함수 구조 리팩토링 |
| `8fa5786` Update main.c | Thread 구조체 정의(`typedef struct`), FCFS 간트 차트 출력 방식 개선, MAX 100으로 확장 |
| `a715303` Update main.c | SJF(비선점) 알고리즘 구현 및 전체 코드 구조 정리 |
| `f6a4ea6` Update main.c | 파일 I/O 기반으로 전환 (`FILE* input/output`), `cpuschedule()` 함수 분리, 테스트 비교 로직 추가 |
| `aa6a3ab` Update main.c | 테스트 결과 비교 코드 작성 — 8개 알고리즘별 PASS/FAIL 판정 로직 |
| `d5ee98d` Update main.c | `main()` 입력 파일 파싱 개선 (`strtok`으로 `<` 처리), 테스트 비교 로직 보완 |
| `4010f00` Update RR.c | Priority (Preemptive) 알고리즘 구현 — `findpriority()`, `PriorityPreemptive()` 작성 |
| `d300bd3` ljfpree | LJF (Preemptive) 알고리즘 구현 — `findLongestJob()`, `LJFpree()` 작성, 작업 도착 시 선점 로직 |
| `a4c145f` Create cpuschedule-test.c | 자동화 테스트 프로그램 작성 — cpuschedule 실행 결과와 기대 출력 비교 |
| `20fd1e8` Update cpuschedule-test.c | 테스트 코드 수정 |
| `f8ceb5d` error | main.c 에러 수정 |
