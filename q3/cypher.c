#include "cypher.h"
#include <stdio.h>
#include <limits.h>
#define MAX_SIZE_QUOTE = 4096;
#define MAX_SIZE_CYPHER = 100;
#define MAX_SIZE_WORD = 50;

typedef struct{
    char word[MAX_SIZE_WORD];
} Word;

Word readCypher(){
    FILE *dictionary;
    dictionary = fopen ("cypher.txt", "wr"); //ver esta parte

    Word cypher[MAX_SIZE_CYPHER][MAX_SIZE_CYPHER];
    int i = 0;
    char word1[MAX_SIZE_WORD];
    char word2[MAX_SIZE_WORD];

    while(word1[0] != EOF){
        fscanf(dictionary,"%s %s", word1, word2);
        cypher[i][1].word = word1;
        cypher[i][2].word = word2;
        i++;
    }
    return cypher;
}

void readAndTransformQuote(Word **cypher){
    // ler aos poucos com scanf as palavras,
    // se estiver no dicionário, printf(palavra substituta)
    char word[MAX_SIZE_WORD];
    char currentChar;
    int i = 0;
    while((currentChar = getchar()) != EOF){
        i = 0;
        while(currentChar != '!' && currentChar != '?' && currentChar != '.' && currentChar != ' '){
            word[i] = currentChar;
            i++;
            currentChar = getchar();
        }
        if(word != {'\n'}){
            for(int j = 0 ; j < sizeof(cypher) ; j++){
                if(word == cypher[j][0].word){ //substitui a palavra pela correspondente no dicionário
                    printf("%s", cypher[j][1].word);
                    word = "";
                }
            }
            if(word != {'\n'}){ //para o caso de ter uma palavra, mas não presente no ficheiro cypher
                printf("%s", word);
            }
        }
        printf("%c", currentChar); //para o caso de não ter uma palavra, mas ter um caracter aleatório sozinho
    }

    //changeCypher - função para alterar o cypher no fim da transformação

    //return quote;
}

int main(int argc, char* argv[]){
    Word cypher[MAX_SIZE_CYPHER][MAX_SIZE_CYPHER] = readCypher();
    readAndTransformQuote(cypher);
    printf("\nProcesso terminado\n");
    return 0;
}