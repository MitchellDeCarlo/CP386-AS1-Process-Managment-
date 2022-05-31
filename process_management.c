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
        system("true > output.txt");
        int pipes[6][2];
        for (int i = 0; i< 6; i++)
        {
            if (pipe(pipes[i]) == -1)
            {
                printf("Error with creating pipe\n");
                return 1;
            }
        }
        pid_t pids[5];
        for (int i = 0; i < 5; i++)
        {
            char **strs = malloc(5 * sizeof(char *));

            for (int i = 0; i < 8; i++)
            {
                strs[i] = (char *)malloc(8);
            }
            filldynamicarr(strs, arr[i]);
            pids[i] = fork();
            if (pids[i] == 0)
            {
                close(pipes[i][READER]);
                dup2(pipes[i][WRITER], STDOUT_FILENO);
                execvp(strs[0], strs);
                return 0;
            }
            else
            {
                wait(NULL);
                char buffer[2046];
                close(pipes[i][WRITER]);

                read(pipes[i][READER], buffer, sizeof(buffer));

                writeoutput(arr[i], buffer);

                memset(buffer, 0, sizeof(buffer));
            }
        }

        for (int i = 0; i < 5; i++)
        {
            wait(NULL);
        }
        return 0;
    }
}
