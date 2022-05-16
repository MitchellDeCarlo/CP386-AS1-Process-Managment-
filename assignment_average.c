#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
  
  
int main(){
    int grades[6][10];
    FILE* fp = fopen("sample_in_grades.txt","r");
    char ch;
    char temp[3];
    int index = 0;
    int index_1= 0;
    int temp_in = 0;

     if(fp == NULL){
        printf("Error in opening file\n");
        exit(0);
    }

    while((ch = fgetc(fp)) != EOF){
        if(isdigit(ch)){
            temp[temp_in] = ch;
            temp_in++;
        }
        if(temp_in > 0 && !isdigit(ch)){
            temp_in = 0;
            grades[index][index_1] = atoi(temp);
            printf("%d [%d] [%d]\n",grades[index][index_1],index,index_1);
            index++;
            temp[0] = '\0';
            temp[1] = '\0';
            temp[2] = '\0';
            if(index > 5){
                index = 0;
                index_1++;
            }
        }
        grades[5][9] = atoi(temp);
    }
    for(int i = 0; i < 6; i++){
        for(int j = 0; j<10; j++){
            printf("%d ",grades[i][j]);
        }
        printf("\n");
    }
    fclose(fp);
    pid_t GradTA = fork();

    if(GradTA == 0){
        sleep(1);
        printf("I am child my parent is %d and I am %d\n",getppid(),getpid());
    }
    else{
        pid_t GradTA_2 = fork();

        if(GradTA_2 == 0){
            sleep(2);
            printf("I am child 2 my parent is %d and I am %d\n",getppid(),getpid());
        }
        else{
            pid_t GradTA_3 = fork();

            if(GradTA_3 == 0){
                sleep(3);
                printf("I am child 3 my parent is %d and I am %d\n",getppid(),getpid());
            }  
             
            else{
                system("ps -l");
                printf("I am parent my id is %d\n",getpid());
                wait(NULL);
            }
        }
    }

    
    return 0;
}
