#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define DELIM " \t\r\n\a"

#define bool int
#define true 1
#define false 0

void shell_loop();
char* read_input();
char** parse_input(char* input);
int execute_command(char** args);
int handle_pipes(char** args, char* in, char* out, int append);
int handle_redirectIn(char** args, char* filename);
int handle_redirectOut(char** args, char* filename, int append);
int handle_redirectInandOut(char** args, char* infile, char* outfile, int append);

int main() {
    shell_loop();
    return 0;
}

void shell_loop() {
    char* input;
    char** args;
    int status = 1;

    do {
        printf("SWU Shell! ");
        input = read_input();
        args = parse_input(input);

        if (args[0] != NULL && strcmp(args[0], "exit") == 0) {
            status = 0;
        }
        else {
            status = execute_command(args);
        }

        free(input);
        free(args);
    } while (status);
}

char* read_input() {
    char* input = NULL;
    ssize_t bufsize = 0;
    getline(&input, &bufsize, stdin);
    return input;
}

char** parse_input(char* input) {
    int bufsize = MAX_ARGS, position = 0;
    char** tokens = malloc(bufsize * sizeof(char*));
    char* token;

    token = strtok(input, DELIM);
    if (!tokens) {
        fprintf(stderr, "dynamic memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "ERROR: buffer allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int execute_command(char** args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            char* in = NULL;
            char* out = NULL;
            int append = 0;
            for (int j = i + 1; args[j] != NULL; j++) {
                if (strcmp(args[j], "<") == 0) {
                    args[j] = NULL;
                    j++;
                    in = args[j];
                }
                if (strcmp(args[j], ">") == 0) {
                    if (strcmp(args[j+1], ">") == 0) {
                        args[j] = NULL;
                        args[j + 1] = NULL;
                        j += 2;
                        out = args[j];
                        append = 1;
                    }
                    else {
                        args[j] = NULL;
                        j++;
                        out = args[j];
                    }
                }
            }
            handle_pipes(args,in,out,append);
        }
        if (strcmp(args[i], "<") == 0) {
            args[i] = NULL;  // '<' 기호 뒤에 파일명만 남도록 설정
            for (int j = i+1; args[j] != NULL; j++) {
                if (strcmp(args[j], ">") == 0) {
                    args[j] = NULL;
                    if (strcmp(args[j + 1], ">") == 0) {
                        args[j + 1] = NULL;
                        handle_redirectInandOut(args, args[i + 1], args[j + 2], 1);
                        return 1;
                    }
                    
                    handle_redirectInandOut(args, args[i + 1], args[j + 1], 0);
                    return 1;
                    
                }
            }
            handle_redirectIn(args, args[i + 1]);
            return 1;  // 루프 계속
        }
        else if (strcmp(args[i], ">") == 0) {
            args[i] = NULL;  // '>' 기호 뒤에 파일명만 남도록 설정
            if (args[i + 1] && strcmp(args[i + 1], ">") == 0) {
                args[i + 1] = NULL;
                handle_redirectOut(args, args[i + 2], 1);
                return 1;  // 루프 계속
            }
            else {
                handle_redirectOut(args, args[i + 1], 0);
                return 1;  // 루프 계속
            }
        }
        else if (strcmp(args[i], ">>") == 0) {
            args[i] = NULL;  // '>>' 기호 뒤에 파일명만 남도록 설정
            handle_redirectOut(args, args[i + 1], 1);
            return 1;  // 루프 계속
        }
    }


    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("ERROR: exec failed");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0) {
        perror("ERROR: fork failed");
    }
    else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;  // 루프 계속
}

int handle_redirectInandOut(char** args, char* infile, char* outfile, int append) {
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        // 입력 파일을 열기
        int in = open(infile, O_RDONLY);
        if (in == -1) {
            perror("ERROR: Failed to open input file");
            exit(EXIT_FAILURE);
        }

        // 출력 파일을 열기 (덮어쓰기)
        int out = -1;
        if (append == 0) {
            out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
        }
        else {
            out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0600);
        }

        if (out == -1) {
            perror("ERROR: Failed to open output file");
            exit(EXIT_FAILURE);
        }

        // 표준 입력을 입력 파일로 리다이렉션
        dup2(in, STDIN_FILENO);
        close(in);

        // 표준 출력을 출력 파일로 리다이렉션
        dup2(out, STDOUT_FILENO);
        close(out);

        // 명령어 실행
        if (execvp(args[0], args) == -1) {
            perror("ERROR: Command not found");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0) {
        perror("ERROR: Fork failed");
    }
    else {
        waitpid(pid, NULL, 0);
    }

    return 0;
}

int handle_redirectIn(char** args, char* filename) {
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        int in = open(filename, O_RDONLY);
        if (in == -1) {
            perror("ERROR: Failed to open input file");
            exit(EXIT_FAILURE);
        }

        // 표준 입력을 리다이렉션할 파일로 변경
        dup2(in, STDIN_FILENO);
        close(in);

        // 명령어 실행
        if (execvp(args[0], args) == -1) {
            perror("ERROR: Command not found");
            exit(127);
        }
    }
    else if (pid < 0) {
        perror("ERROR: Fork failed");
    }
    else {
        waitpid(pid, NULL, 0);
    }

    return 0;
}

int handle_redirectOut(char** args, char* filename, int append) {
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        // filename은 출력 리다이렉션할 파일명이어야 합니다.
        int out = -1;
        if (append == 0) {
            out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
        }
        else {
            out = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0600);
        }

        if (out == -1) {
            perror("ERROR: Failed to open output file");
            exit(EXIT_FAILURE);
        }

        // 표준 출력을 리다이렉션할 파일로 변경
        dup2(out, STDOUT_FILENO);
        close(out);

        // 명령어 실행
        if (execvp(args[0], args) == -1) {
            perror("ERROR: Command not found");
            exit(127);
        }
    }
    else if (pid < 0) {
        perror("ERROR: Fork failed");
    }
    else {
        waitpid(pid, NULL, 0);
    }

    return 0;
}

int handle_pipes(char** args, char* in, char* out, int append) {
    int fd[2];
    pid_t pid1, pid2;

    int i = 0;
    while (args[i] != NULL && strcmp(args[i], "|") != 0) {
        i++;
    }
    if(args[i]!=NULL){
	args[i]=NULL;
    }

    if (pipe(fd) == -1) {
        perror("ERROR: Pipe failed");
        return 1;
    }

    pid1 = fork();
    if (pid1 == 0) {
        int infile;
        if (in != NULL) {
            infile = open(in, O_RDONLY);
	    if(infile== -1){
		perror("ERROR: Failed to open input file");
		exit(EXIT_FAILURE);
	    }
            dup2(infile, STDIN_FILENO);
	    close(infile);
        }
        dup2(fd[1], STDOUT_FILENO);  // Write to pipe
        close(fd[0]);
        close(fd[1]);
        if (execvp(args[0], args) == -1) {
            perror("ERROR: exec failed for first command");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid1 < 0) {
        perror("ERROR: Fork failed");
        return 1;
    }

    pid2 = fork();
    if (pid2 == 0) {
        int outfile;
        if (out != NULL) {
            if (append == 1) {
                outfile = open(out, O_WRONLY | O_CREAT | O_APPEND, 0600);
            }
            else {
                outfile = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0600);
            }
	    if(outfile==-1){
		perror("ERROR: Failed to open output file");
		exit(EXIT_FAILURE);
	    }
            dup2(outfile, STDOUT_FILENO);  // Write to output file
	    close(outfile);
        }
        dup2(fd[0], STDIN_FILENO);  // Read from pipe
        close(fd[1]);
        close(fd[0]);
        if (execvp(args[i + 1], &args[i + 1]) == -1) {  // Second command
            perror("ERROR: exec failed for second command");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid2 < 0) {
        perror("ERROR: Fork failed");
        return 1;
    }

    close(fd[0]);
    close(fd[1]);

    int status;
    while (wait(&status) > 0);

    return 1;
}

