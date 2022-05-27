#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BUFFER_SIZE 1024

void readFile(FILE* input, char string[]){
    char character = fgetc(input);
    int i = 0;
    while((character != '!' && character != '?' && character != '.') && !feof(input)){
        string[i] = character;
        i++;
        character = fgetc(input); // GET NEW CHARACTER
    }
    while(character == '!' || character == '?' || character == '.') {
        string[i] = character;
        i++;
        character = fgetc(input); // GET NEW CHARACTER
    }
    if (isspace(character)){
        string[i] = character;
        i++;
    }
    string[i] = '\0';
    return;
}


int main(int argc, char* argv[]) {
    FILE* input;
    if (argc == 2) {
        input = fopen(argv[1], "r");
    }
    else if (argc == 3 && !strcmp(argv[1],"-l")) {
        input = fopen(argv[2], "r");
    }
    else{
        printf("usage: phrases [-l] file\n");
        return EXIT_FAILURE;
    }
    if (input == NULL) {
        printf("Unable to open the file\n");
        return -1;
    }
    char string[BUFFER_SIZE];
    int lineNum = 0;
    while(!feof(input)){
        readFile(input, string);
        if (strcmp(argv[1],"-l") == 0){
            printf("[%d] ",lineNum+1);
            int j = 0;
            while(string[j] != '\0'){ 
                printf("%c",string[j]);
                j++;
            }
            printf("\n");
        }
        lineNum ++;
    }
    fclose(input); // CLOSE FILE
    if (argc == 2) {
        printf("%d\n",lineNum);
    }
    return 0;
}
