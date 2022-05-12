#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <unistd.h>
  
  
int main(){

    char command[10];
    char zombrun[20];
    char find[70];
    char pidnum[7];
    strcpy( zombrun, "./z_creator &" );
    strcpy( command, "ps -l" );
    strcpy(find, "ps -l| grep -w Z |tr -s ''|cut -d '' -f 5 > out.txt");
    system(zombrun);
    system(command);
    system(find);

    FILE *fp;
    char line[300];
    fp = fopen("out.txt","r");

    fclose(fp);

    return 0;
}
