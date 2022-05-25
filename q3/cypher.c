#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

char getWord(char *word){
    char currentChar;
    int i = 0;
    currentChar = getchar();
    if(currentChar == EOF){
        word[0] = '\0';
        return '\0';
    }
    while(isspace(currentChar)==0 && currentChar != '!'
          && currentChar != '?'&& currentChar != '.' && currentChar != '-'&& currentChar != EOF){
        word[i] = currentChar;
        i++;
        currentChar = getchar();
    }
    word[i] = '\0';
    return currentChar;
}

void readAndTransformQuote(Cypher *cypher, int sizeCypher){
    // ler aos poucos com scanf as palavras,
    // se estiver no dicionário, printf(palavra substituta)
    char word[MAX_SIZE_WORD] = {'\0'};
    char sep;
    int isPrinted = 0;
    int endoffile = 0;
    while(1){
        isPrinted = 0;
        sep = getWord(word);
        if((word[0]=='\0' && sep=='\0')){
            endoffile = 1;
        }
        for(int j = 0 ; j < sizeCypher; j++){
            if(strcmp(word,cypher[j].wordToSwitch1) == 0){ //substitui a palavra pela correspondente no dicionário
                printf("%s", cypher[j].wordToSwitch2);
                printf("%c", sep);
                isPrinted = 1;
            }
            else if(strcmp(word,cypher[j].wordToSwitch2) == 0){ //substitui a palavra pela correspondente no dicionário
                printf("%s", cypher[j].wordToSwitch1);
                printf("%c", sep);
                isPrinted = 1;
            }
        }
        if(!isPrinted){ //para o caso de ter uma palavra, mas não presente no ficheiro cypher
            printf("%s", word);
            printf("%c", sep);
        }
        if(endoffile == 1){
            break;
        }
    }
}

int main(int argc, char* argv[]){
    Cypher cypher[MAX_SIZE_CYPHER];
    int sizeCypher;
    sizeCypher = readCypher(cypher);
    readAndTransformQuote(cypher, sizeCypher);
    printf("\nProcesso terminado\n");
    return 0;
}
