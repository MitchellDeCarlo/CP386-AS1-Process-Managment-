#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
  
  
int main(){

    char command[10];
    char zombrun[20];
    char find[70];
    char pidnum[7];
    char finalcom[12];

    strcpy(finalcom,"kill -9 ");
    strcpy( zombrun, "./z_creator &" );
    strcpy( command, "ps -l" );
    strcpy(find, "ps -l| grep -w Z |tr -s ''|cut -d '' -f 5 > out.txt");
    system(zombrun);
    sleep(2);
    system(command);
    system(find);

    FILE* fp = NULL;
    char filename[10];
    strcpy(filename,"out.txt");
    
    char ch;
    fp = fopen(filename,"r");

    int count = 0;
    int index = 0;

    if(fp == NULL){
        printf("Error in opening file\n");
        exit(0);
    }

    while((ch = fgetc(fp)) != EOF){
       
        if(count >= 17 && count <= 20){
            pidnum[index] = ch;
            index++;
        }
        count++;
    }

    strcat(finalcom,pidnum);

    printf("\n");
    printf("The updated list of processes and their status is:\n");
    system(finalcom);
    system(command);

    fclose(fp);

    return 0;
}
