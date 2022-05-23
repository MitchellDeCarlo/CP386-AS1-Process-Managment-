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

int main(int argc, char *argv[])
{
    void *ptr;
    int shm_fd;
    const int SIZE = 4096;
    const char *name = "OS";
    pid_t child1 = fork();
    char comm[100];
    if (child1 == 0)
    {
        FILE *fp = fopen(argv[1], "r");
        char ch;
        int index = 0;
        if (fp == NULL)
        {
            printf("Error opening file.\n");
        }
        else
        {
            while ((ch = fgetc(fp)) != EOF)
            {
                comm[index++] = ch;
            }
        }
        fclose(fp);

        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SIZE);
        ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        sprintf(ptr, "%s", comm);
        ptr += strlen(comm);
    }
    else
    {
        wait(NULL);
        pid_t child2 = fork();

        if (child2 == 0)
        {
            shm_fd = shm_open(name, O_RDONLY, 0666);
            ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
            char commands[40];
            char temp[20];
            strcpy(commands, (char *)ptr);

            char **arr = malloc(5 * sizeof(char*));

            for (int i =0; i < 5; i++){
                arr[i] = (char *)malloc(30 * sizeof(char));
            }
            int iter = 0;
            int iter2 = 0;
            for (int i = 0; i < strlen(commands); i++)
            {
                if (commands[i] != '\n')
                {
                    
                    temp[iter++] = commands[i];
                }
                else
                {
                   // printf("%s\n",temp);
                    arr[iter2++] = temp;
                    for (int i = 0; i < 20; i++)
                    {
                        temp[i] = '\0';
                    }
                    iter =0;
                }
            }

            for(int i =0; i < 5; i++){
               printf("%s\n",arr[i]);
            }

            shm_unlink(name);

            free(arr);
        }
    }

    return 0;
}
