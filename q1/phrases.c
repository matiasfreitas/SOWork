#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

char[] readFile(FILE* file){
    if(input == NULL){
        printf("Não foi possível abrir o ficheiro");
        return -1;
    }
    char[BUFFER_SIZE][BUFFER_SIZE] string = {};
    int i = 0, j = 0;
    char c = fgetc(input);
    while(c != EOF){
        j = 0;
        while(c != '!' && c != '?' && c != '.'){
            string[i][j] = c;
            //printf("%c", c);
            c = fgetc(input);
            j++;
        }
        // a partir daqui temos pontos de terminação de frases
        // não sei se cobre a parte das reticências mas penso que sim
        string[i][j] = c;
        i++;
    }
}

int main(int argc, char* argv[]) {
    /* check if there is no argument */
    if(argc == 1) {
      printf("usage: phrases [-l] file\n");
      return EXIT_FAILURE;
    } else if(argc == 2){
        FILE* input = fopen(argv[1], "r");

        char[BUFFER_SIZE][BUFFER_SIZE] string = readFile(input);

        fclose(input);
    } else if(argc == 3) {
        FILE* input = fopen(argv[2], "r");

        char[BUFFER_SIZE][BUFFER_SIZE] string = readFile(input);

        fclose(input);

        for(int i = 0 ; ?? ; i++){
            print("[%d] ",i+1);
            for(int j = 0 ; string[i][j] != '\0' ; j++){
                printf("%c", string[i][j]);
            }
            printf("\n");
        }
    }


    return EXIT_SUCCESS;
}