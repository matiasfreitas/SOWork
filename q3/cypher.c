#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define READ_END 0 // PONTAS DA PIPE
#define WRITE_END 1 // PONTAS DA PIPE

#define BUFF_SIZE 256 // BUFFER SIZE (MAXIMUM SIZE OF ARRAY)
int parent(int pipe_1[2]) { 
    close(pipe_1[READ_END]);

    char *string;
    string = malloc(BUFF_SIZE);

    int bytesin;
    while ((bytesin = read(STDIN_FILENO, string, BUFF_SIZE))>0) { // CHECK IF READING
        write(pipe_1[WRITE_END], string, bytesin);
    }

    close(pipe_1[WRITE_END]);
    return 0;
}

char * cypherString(char *string) {
    
}

int child(int pipe_1[2], int pipe_2[2]) {
    close(pipe_1[WRITE_END]);
    close(pipe_2[READ_END]);

    char *string;
    string = malloc(BUFF_SIZE);

    int bytesin;
    while ((bytesin = read(pipe_1[READ_END], string, BUFF_SIZE))>0) { // CHECK IF READING
        char *stringcyphered = cypherString(string);
        write(pipe_2[WRITE_END], stringcyphered, bytesin);
    }

    close(pipe_1[READ_END]);
    close(pipe_2[WRITE_END]);
    return 0;
}

int parent2(int pipe_2[2]) {
    close(pipe_2[WRITE_END]);
    
    char *string;
    string = malloc(BUFF_SIZE);

    int bytesin;
    while((bytesin = read(pipe_2[READ_END], string, BUFF_SIZE))>0) {
        write(STDOUT_FILENO, string, bytesin);
    }

    close(pipe_2[READ_END]);
    return 0;
}

int main(int argc, char* argv[]){
    if (argc != 1) {
        return EXIT_FAILURE;
    }

    int pipe_dad_to_child[2];
    int pipe_child_to_dad[2];

    if (pipe(pipe_child_to_dad) < 0) {
        return EXIT_FAILURE;
    }
    if (pipe(pipe_dad_to_child) < 0) {
        return EXIT_FAILURE;
    }

    pid_t pid;
    if ((pid = fork()) == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    else if (pid == 0) {
        /* child process */
        child(pipe_dad_to_child, pipe_child_to_dad);
        return EXIT_SUCCESS;
    }
    else {
        /* parent process */
        parent(pipe_dad_to_child);
        if (waitpid(pid, NULL, 0) == -1) {
            perror("wait");
            return EXIT_FAILURE;
        }
        parent2(pipe_child_to_dad);
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;

}