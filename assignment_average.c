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
    return 0;
}
