#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define READ_END 0 // PONTAS DA PIPE
#define WRITE_END 1 // PONTAS DA PIPE

#define BUFF_SIZE 1000 // BUFFER SIZE (MAXIMUM SIZE OF ARRAY)

#define MAX_SIZE_CYPHER 100
#define MAX_SIZE_WORD 50

typedef struct{
    char wordToSwitch1[MAX_SIZE_WORD];
    char wordToSwitch2[MAX_SIZE_WORD];
} Cypher;

int readCypher(Cypher* cypher){
    FILE *dictionary;
    dictionary = fopen ("cypher.txt", "r"); // OPEN FILE
    //change here to use another dictionary
    int i = 0;
    char word1[MAX_SIZE_WORD];
    char word2[MAX_SIZE_WORD];

    while(fscanf(dictionary,"%s %s", word1, word2)!=EOF){ 
        strcpy(cypher[i].wordToSwitch1,word1);
        strcpy(cypher[i].wordToSwitch2,word2);
        i++;
    }
    fclose(dictionary);
    return i;

}
int parent(int pipe_1[2]) { 
    close(pipe_1[READ_END]);

    char *string;
    string = malloc(BUFF_SIZE);

    int bytesin;
    while ((bytesin = read(STDIN_FILENO, string, BUFF_SIZE))>0) { // CHECK IF READING
        write(pipe_1[WRITE_END], string, bytesin);
    }

    if (bytesin == -1) {  // ERROR
        fprintf(stderr, "Error reading from pipe! %s\n", strerror(errno));
        close(pipe_1[WRITE_END]);
        return EXIT_FAILURE;
    }

    close(pipe_1[WRITE_END]);
    return EXIT_SUCCESS;
}


char* string_replace(char* substring_source, int substring, char* with) {
    if (substring_source == NULL) {
        return NULL;
    }
 
    memmove(
        substring_source + strlen(with),
        substring_source + substring,
        strlen(substring_source) - substring + 1
    );
 
    memcpy(substring_source, with, strlen(with));
    return substring_source + strlen(with);
}

int child(int pipe_1[2], int pipe_2[2],Cypher *cypher, int sizeCypher) {
    close(pipe_1[WRITE_END]);
    close(pipe_2[READ_END]);

    char *string;

    string = malloc(BUFF_SIZE); // ALLOCATE MEMORY FOR STRING

    int bytesin;
    while ((bytesin = read(pipe_1[READ_END], string, BUFF_SIZE))>0) { // CHECK IF READING
        char word[MAX_SIZE_WORD]={'\0'};
        char *beginingofword = string;
        char *text = string;
        int i = 0;
        for (; *text!='\0'; text++){
            if(!((*text >= 'a' && *text <= 'z') || (*text >= 'A' && *text <= 'Z')|| (*text >= '1' && *text <= '9'))){
                i = 0;
                for(int j = 0 ; j < sizeCypher; j++){
                    if(strcmp(word,cypher[j].wordToSwitch1) == 0){ //REPLACE WORD BY CORRESPONDING IN THE DICTIONARY
                        text = string_replace(beginingofword+1,strlen(word),cypher[j].wordToSwitch2);
                    }
                    else if(strcmp(word,cypher[j].wordToSwitch2) == 0){ //REPLACE WORD BY CORRESPONDING IN THE DICTIONARY
                       text = string_replace(beginingofword+1,strlen(word),cypher[j].wordToSwitch1);
                    }
                }
                for(int k = 0;k<MAX_SIZE_WORD;k++){ 
                    word[k]='\0';
                }
                beginingofword = text;
            }
            else{
               word[i]=*text; 
               i++; 
            }
        }
        write(pipe_2[WRITE_END], string, strlen(string)); // WRITE TO THE SECOND PIPE
    }

    if (bytesin == -1) { // ERROR
        fprintf(stderr, "Error reading from pipe! %s\n", strerror(errno));
        close(pipe_1[READ_END]);
        close(pipe_2[WRITE_END]);
        return EXIT_FAILURE;
    }

    close(pipe_1[READ_END]);
    close(pipe_2[WRITE_END]);
    return EXIT_SUCCESS;
}

int parent2(int pipe_2[2]) {
    close(pipe_2[WRITE_END]); // ONLY READS
    
    char *string;
    string = malloc(BUFF_SIZE);

    int bytesin;
    while((bytesin = read(pipe_2[READ_END], string, BUFF_SIZE))>0) { // CHECK IF READING
        write(STDOUT_FILENO, string, strlen(string));
    }

    if (bytesin == -1) {
        fprintf(stderr, "Error reading from pipe: %s\n", strerror(errno));
        close(pipe_2[READ_END]);
        return EXIT_FAILURE;
    }

    close(pipe_2[READ_END]); // NO NEED FOR READ END
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]){
    if (argc != 1) {
        return EXIT_FAILURE;
    }
     Cypher cypher[MAX_SIZE_CYPHER];
    int sizeCypher;
    sizeCypher = readCypher(cypher); 

    int pipe_dad_to_child[2];
    int pipe_child_to_dad[2];

    if (pipe(pipe_child_to_dad) < 0) { // CAN'T BE NEGATIVE
        return EXIT_FAILURE;
    }
    if (pipe(pipe_dad_to_child) < 0) { // CAN'T BE NEGATIVE
        return EXIT_FAILURE;
    }

    pid_t pid;
    if ((pid = fork()) < 0) { // CAN'T BE NEGATIVE
        perror("fork");
        return EXIT_FAILURE;
    }
    else if (pid == 0) { // IF PROCESS IS CHILD
        child(pipe_dad_to_child, pipe_child_to_dad,cypher,sizeCypher);
        return EXIT_SUCCESS;
    }
    else { // IF PROCESS IS PARENT
        parent(pipe_dad_to_child);
        if (waitpid(pid, NULL, 0) < 0) { // COULDN'T WAIT
            fprintf(stderr, "Error waiting for child: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }

        parent2(pipe_child_to_dad); 
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;

}