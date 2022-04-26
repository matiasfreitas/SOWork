#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFFER_SIZE 1024

int next_block_size(int count, int buffer_size) {
    return (count >= buffer_size)? buffer_size: count % buffer_size;
}

int main(int argc, char* argv[]) {
    /* check if there is no argument */
    if (argc == 1) {
      printf("usage: phrases [-l] file\n");
      return EXIT_FAILURE;
    }

    if(argc == 2){
        FILE* input = fopen(argv[1], "r");
        if(input == NULL){
            printf("Não foi possível abrir o ficheiro");
            return -1;
        }
        char c = fgetc(input);
        while(c != EOF){
            printf("%c", c);
            c = fgetc(input);
        }
        fclose(input);
    }

    return EXIT_SUCCESS;
}