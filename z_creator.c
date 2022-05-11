#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
  
int main()
{
    int proc_pid = fork();
    
    if (proc_pid > 0){
        printf("In parent process!\n");
    }
    else if (proc_pid == 0){
        printf("In child process");
        sleep(5);
    }

    return 0;
}