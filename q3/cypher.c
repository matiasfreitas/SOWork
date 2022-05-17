#include "cypher.h"
#include <stdio.h>
#define MAX_SIZE_QUOTE = INT_MAX;
#define MAX_SIZE_CYPHER = INT_MAX;

char[][] readCypher(){
    char[MAX_SIZE_CYPHER][MAX_SIZE_CYPHER] cypher = {};
    int i = 0;
    char word1[] = {}, word2[] = {};

    while(word1 != EOF && word2 != EOF){
        scanf("%s %s", word1, word2);
        cypher[i][1] = word1;
        cypher[i][2] = word2;
        i++;
    }
    return cypher;
}

char[][] readAndTransformQuote(char[][] cypher){
    // ler aos poucos com scanf as palavras,
    // se estiver no dicionário, printf(palavra substituta)
    char[] word = {};
    char c;
    int i = 0;
    while((c = getchar()) != EOF){
        i = 0;
        while(c != '!' && c != '?' && c != '.' && c != ' ') {
            word[i] = c;
            i++;
            c = getchar();
        }
        if(word != {}){
            for(int i = 0 ; i < sizeof(cypher) ; i++){
                if(word == cypher[i][1]){
                    printf("%s", cypher[i][2]);
                    word = {};
                }
            }
        } else {
            printf("%c", c);
        }
    }

    //changeCypher - função para alterar o cypher no fim da transformação

    return quote;
}

int main(int argc, char* argv[]){
    char[][] cypher = readCypher();
    char[][] quote = readAndTransformQuote(cypher);
}