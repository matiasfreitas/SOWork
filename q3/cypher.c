#include "cypher.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE_QUOTE 4096
#define MAX_SIZE_CYPHER 100
#define MAX_SIZE_WORD 50

typedef struct{
    char wordToSwitch1[MAX_SIZE_WORD];
    char wordToSwitch2[MAX_SIZE_WORD];
} Cypher;

int readCypher(Cypher** cypher){
    FILE *dictionary;
    dictionary = fopen ("cypher.txt", "r"); //ver esta parte
    int i = 0;
    char word1[MAX_SIZE_WORD];
    char word2[MAX_SIZE_WORD];

    while(fscanf(dictionary,"%s %s", word1, word2) == 1){
        strcpy(cypher[i]->wordToSwitch1,word1);
        strcpy(cypher[i]->wordToSwitch2,word2);
        i++;
    }
    return i;

}

void getWord(char *word){
    char currentChar = '\0';
    int i = 0;
    currentChar = getchar();
    while(currentChar != '!' && currentChar != '?' && currentChar != '.'
          && isspace(currentChar) &&  currentChar != '\0'){
        word[i] = currentChar;
        i++;
        currentChar = getchar();
    }
    word[i+1] = '\0';
}

void readAndTransformQuote(Cypher **cypher, int sizeCypher){
    // ler aos poucos com scanf as palavras,
    // se estiver no dicionário, printf(palavra substituta)
    char word[MAX_SIZE_WORD];

    while(1){
        getWord(word);
        if(word[0] == '\0'){
            break;
        }
        for(int j = 0 ; j < sizeCypher; j++){
            if(strcmp(word,cypher[j]->wordToSwitch1) == 0){ //substitui a palavra pela correspondente no dicionário
                printf("%s", cypher[j]->wordToSwitch2);
                word[0] = '\0';//word = "";
            }
            else if(strcmp(word,cypher[j]->wordToSwitch2) == 0){ //substitui a palavra pela correspondente no dicionário
                printf("%s", cypher[j]->wordToSwitch1);
                word[0] = '\0';//word = "";
            }
        }
        if(word[0] != '\0'){ //para o caso de ter uma palavra, mas não presente no ficheiro cypher
            printf("%s", word);
        }
    }

    //changeCypher - função para alterar o cypher no fim da transformação

    //return quote;
}

int main(int argc, char* argv[]){
    Cypher* cypher[MAX_SIZE_CYPHER];
    int sizeCypher;
    for (int i = 0; i < MAX_SIZE_CYPHER; ++i) {
        cypher[i] = (Cypher[MAX_SIZE_CYPHER]){};
    }

    sizeCypher = readCypher(cypher);
    readAndTransformQuote(cypher, sizeCypher);
    printf("\nProcesso terminado\n");
    return 0;
}