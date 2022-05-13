#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <unistd.h>
  
int main()
{
    pid_t proc_pid = fork();
    
    if (proc_pid > 0){
        printf("In parent process!\n");
         sleep(10);
       
    }
    else{
        printf("In child process!\n");
        exit(0);
    }

    return 0;
}