#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc,char* argv[])
{
    void *ptr;
    int shm_fd;
    const int SIZE = 4096;
    const char *name = "OS"; 
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
        fclose(fp);

        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd,SIZE);
        ptr = mmap(0,SIZE,PROT_WRITE,MAP_SHARED,shm_fd,0);

        sprintf(ptr,"%s",comm);
        ptr += strlen(comm);
    }
    else{
        wait(NULL);
        
        shm_fd = shm_open(name, O_RDONLY, 0666);

        ptr = mmap(0,SIZE,PROT_READ,MAP_SHARED,shm_fd,0);

        printf("%s\n",(char *)ptr);

        shm_unlink(name);
        
    }

    return 0;
}
