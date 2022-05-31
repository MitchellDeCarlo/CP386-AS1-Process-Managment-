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

#define BUFFER 25
#define READER 0
#define WRITER 1

void writeoutput(char *command, char *output)
{
    FILE *fp;
    fp = fopen("output.txt", "a");
    fprintf(fp, "The output of: %s : is\n", command);
    fprintf(fp, ">>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<\n", output);
    fclose(fp);
}

void filldynamicarr(char **strs, char *arr)
{
    char temp[8];
    int tempin = 0;
    int indexer = 0;
    for (int j = 0; j < strlen(arr) + 1; j++)
    {
        if (arr[j] != ' ')
        {
            if (arr[j] == '\0')
            {
                tempin = 0;
                // printf("%s\n", temp);
                strcpy(strs[indexer++], temp);
                for (int k = 0; k < 8; k++)
                {
                    temp[k] = '\0';
                }
            }
            else
            {
                temp[tempin++] = arr[j];
                temp[tempin] = '\0';
            }
        }
        else
        {
            tempin = 0;
            // printf("%s\n", temp);
            strcpy(strs[indexer++], temp);
            for (int k = 0; k < 8; k++)
            {
                temp[k] = '\0';
            }
        }
    }
    for (int j = indexer; j < 5; j++)
    {
        strs[j] = NULL;
    }
}

int main(int argc, char *argv[])
{
    void *ptr;
    int shm_fd;
    const int SIZE = 4096;
    const char *name = "OS";
    char write_msg[BUFFER];
    char read_msg[BUFFER];

    int fd[2];

    if (pipe(fd) == -1)
    {
        return -1;
    }

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
        exit(0);
    }
    else
    {
        wait(NULL);
        shm_fd = shm_open(name, O_RDONLY, 0666);
        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        char commands[40];
        char temp[20];
        strcpy(commands, (char *)ptr);

        char **arr = malloc(5 * sizeof(char *));

        for (int i = 0; i < 5; i++)
        {
            arr[i] = (char *)malloc(10);
        }
        shm_unlink(name);
        int iter = 0;
        int iter2 = 0;
        for (int i = 0; i < strlen(commands) + 1; i++)
        {
            if (commands[i] != '\n')
            {
                if (commands[i] == '\0')
                {
                    // printf("%s\n", temp);
                    strcpy(arr[iter2++], temp);
                    for (int j = 0; j < 20; j++)
                    {
                        temp[j] = '\0';
                    }
                    iter = 0;
                }
                else
                {
                    temp[iter++] = commands[i];
                    temp[iter] = '\0';
                }
            }
            else
            {
                // printf("%s\n", temp);
                strcpy(arr[iter2++], temp);
                for (int j = 0; j < 20; j++)
                {
                    temp[j] = '\0';
                }
                iter = 0;
            }
        }
        pid_t child2 = fork();

        if (child2 == 0)
        {
            // char *command = "ls";
            // char *argument_list[] = {"ls", "-l",NULL,NULL};
            // close(fd[0]);
            // dup2(fd[1], 1);
            // execvp(command, argument_list);

            for (int i = 0; i < 5; i++)
            {
                // printf("%s\n",arr[i]);
                char **strs = malloc(5 * sizeof(char *));

                for (int i = 0; i < 5; i++)
                {
                    strs[i] = (char *)malloc(8);
                }

                filldynamicarr(strs,arr[i]);

                execvp(strs[0], strs);
            }

            exit(0);
        }
        else
        {
            // close(fd[1]); // parent doesn't write

            // char reading_buf[1];
            // while (read(fd[0], reading_buf, 1) > 0)
            // {
            //     write(1, reading_buf, 1); // 1 -> stdout
            // }
            free(arr);
            // close(fd[0]);
            wait(NULL);
        }
    }
    return 0;
}
