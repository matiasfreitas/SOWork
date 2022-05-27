#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_SIZE_CYPHER 100
#define MAX_SIZE_WORD 50

typedef struct{
    char wordToSwitch1[MAX_SIZE_WORD];
    char wordToSwitch2[MAX_SIZE_WORD];
} Cypher;

int readCypher(Cypher* cypher){
    FILE *dictionary;
    dictionary = fopen ("cypher.txt", "r"); //ver esta parte
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

void getWord(char *word){
    char currentChar;
    int i = 0;
    currentChar = getchar();
    if(currentChar == EOF){
        word[0] = '\0';
        return;
    }
    while(isspace(currentChar)==0 && currentChar != '!'
          && currentChar != '?'&& currentChar != '.' && currentChar != '-'&& currentChar != EOF){
        word[i] = currentChar;
        i++;
        currentChar = getchar();
    }
    word[i] = '\0';
    char last[1];
    last[0]= currentChar;
    strcat(word,last);
}






void readAndTransformQuote(Cypher *cypher, int sizeCypher){
    int parent_fds[2], child_fds[2];
    char word[MAX_SIZE_WORD] = {'\0'};
    int isPrinted = 0;
    int endoffile = 0;
    getWord(word);
    while(word[0]!='-'){
    // read:[0] - write:[1]
    if (pipe(parent_fds) != 0 || pipe(child_fds) != 0)  /* || not && */
    {
        fprintf(stderr, "pipes failed!\n");
        return EXIT_FAILURE;
    }

    // fork() child process
    int child = fork();

    if (child < 0)
    {
        fprintf(stderr, "fork failed!");
        return EXIT_FAILURE;
    }
    else if (child == 0)
    {
        //printf("%d: I reached the child :)\n", (int)getpid());
        // close unwanted pipe ends by child
        close(child_fds[0]);
        close(parent_fds[1]);

        // read from parent pipe
        char fromParent[100];
        int n = read(parent_fds[0], fromParent, sizeof(fromParent) - 1);
        fromParent[n] = '\0';
        //printf("%d: Child: read from parent pipe '%s'\n", (int)getpid(), fromParent);
        close(parent_fds[0]);

        // Append to what was read in

        write(child_fds[1], fromParent, strlen(fromParent));
        close(child_fds[1]);
        //printf("%d: Child: writing to pipe - '%s'\n", (int)getpid(), fromParent);
        exit(EXIT_SUCCESS);
    }
    else
    {
        // close unwanted pipe ends by parent
        close(parent_fds[0]);
        close(child_fds[1]);

        // write from terminal to parent pipe FOR child to read
        //printf("%d: Parent: writing to pipe '%s'\n", (int)getpid(),word);
        write(parent_fds[1], word, strlen(word));
        close(parent_fds[1]);
        // read from child pipe
        char fromChild[100];
        int n = read(child_fds[0], fromChild, sizeof(fromChild) - 1);
        fromChild[n] = '\0';
        close(child_fds[0]);

        getWord(word);
    }

    int corpse;
    int status;
    while ((corpse = wait(&status)) > 0){
        printf(" child PID %d exited with status %s\n", (int)getpid(), word);
        }


    }




/*
    // ler aos poucos com scanf as palavras,
    // se estiver no dicionário, printf(palavra substituta)
    char word[MAX_SIZE_WORD] = {'\0'};
    char sep;
    int isPrinted = 0;
    int endoffile = 0;
    while(1){
        isPrinted = 0;
        sep = getWord(word);
        if((word[0]=='\0' && sep=='\0')||sep==EOF){
            endoffile = 1;
        }
        for(int j = 0 ; j < sizeCypher; j++){
            if(strcmp(word,cypher[j].wordToSwitch1) == 0){ //substitui a palavra pela correspondente no dicionário
                printf("%s", cypher[j].wordToSwitch2);
                if(endoffile==0){
                   printf("%c", sep);
                }
                isPrinted = 1;
            }
            else if(strcmp(word,cypher[j].wordToSwitch2) == 0){ //substitui a palavra pela correspondente no dicionário
                printf("%s", cypher[j].wordToSwitch1);
                if(endoffile==0){
                   printf("%c", sep);
                }
                isPrinted = 1;
            }
        }
        if(!isPrinted){ //para o caso de ter uma palavra, mas não presente no ficheiro cypher
            printf("%s", word);
            if(endoffile==0){
                printf("%c", sep);
            }
        }
        if(endoffile == 1){
            break;
        }
    }*/
}

int main(int argc, char* argv[]){
    Cypher cypher[MAX_SIZE_CYPHER];
    int sizeCypher;
    sizeCypher = readCypher(cypher);
    readAndTransformQuote(cypher, sizeCypher);
    printf("\nProcesso terminado\n");
    return 0;
}
