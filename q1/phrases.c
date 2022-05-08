#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BUFFER_SIZE 1024

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
    char string[BUFFER_SIZE][BUFFER_SIZE];
    int linenum = 0, j = 0;
    char lastchar;

    do
    {
        char c = fgetc(input);
        if (feof(input)){
            if(lastchar != '!' && lastchar != '?' && lastchar != '.'){
                linenum++;
            }
            break;
        }
        string[linenum][j] = c;
        if (!isspace(c)){
            lastchar = c;
        }
        if(c == '!' || c == '?' || c == '.'){
            FILE* temp = input;
            int ret = 0;
            while((c = fgetc(input)) == '.'){
                ret = 1;
                j++;
                string[linenum][j] = c;
            }
            if(ret== 0){
                input = temp;
            }
            string[linenum][j+1] = '\0';
            linenum++;
            j=0;
            continue;
        }
        j++;
    }  while(1);
    fclose(input);
    if (argc == 2) {
        printf("%d\n",linenum);
    }
    else{
        for(int i = 0 ; i < linenum ; i++){
            printf("[%d] ",i+1);
            int j = 0;
            while(string[i][j] != '\0'){
                printf("%c",string[i][j]);
                j++;
            }
            printf("\n");
        }
    }
    return EXIT_SUCCESS;
}
