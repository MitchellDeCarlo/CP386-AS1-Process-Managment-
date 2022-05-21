#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

int main(int argc,char* argv[])
{
    pid_t child1 = fork();
    char comm[100];
    if(child1 == 0){
        FILE *fp = fopen(argv[1],"r");
        char ch;
        int index = 0;
        if (fp == NULL){
            printf("Error opening file.\n");
        }
        else{
            while ((ch = fgetc(fp)) != EOF){
                comm[index++] = ch;
            }
        }
        printf("%s",comm);
        fclose(fp);
    }
    else{
        wait(NULL);
        
    }

    return 0;
}
