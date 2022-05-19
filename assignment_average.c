#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

int main()
{
    int grades[6][10];

    pid_t GradTA, GradTA_2, GradTA_3;

    FILE *fp = fopen("sample_in_grades.txt", "r");
    char ch;
    char temp[3];
    int index = 0;
    int index_1 = 0;
    int temp_in = 0;

    int sum = 0;
    float finalavg = 0;

    if (fp == NULL)
    {
        printf("Error in opening file\n");
        exit(0);
    }

    while ((ch = fgetc(fp)) != EOF)
    {
        if (isdigit(ch))
        {
            temp[temp_in] = ch;
            temp_in++;
        }
        if (temp_in > 0 && !isdigit(ch))
        {
            temp_in = 0;
            grades[index][index_1] = atoi(temp);
            index++;
            temp[0] = '\0';
            temp[1] = '\0';
            temp[2] = '\0';
            if (index > 5)
            {
                index = 0;
                index_1++;
            }
        }
        grades[5][9] = atoi(temp);
    }
    fclose(fp);
    GradTA = fork();

    if (GradTA == 0)
    {
        //printf("GradTA PID: %d PPID: %d\n", getpid(), getppid());

        pid_t TA = fork();

        if (TA == 0)
        {
           // printf("TA PID: %d PPID: %d\n", getpid(), getppid());
            for (int i = 0; i < 10; i++)
            {
                sum += grades[0][i];
            }
            finalavg = (float)sum / 10;
            printf("Assignment 1 - Average = %f\n", finalavg);
            sum = 0;
            finalavg = 0;
            exit(0);
        }

        if (TA > 0)
        {
            pid_t TA_2 = fork();
            if (TA_2 == 0)
            {
                //printf("TA 2 PID: %d PPID: %d\n", getpid(), getppid());
                for (int i = 0; i < 10; i++)
                {
                    sum += grades[1][i];
                }
                finalavg = (float)sum / 10;
                printf("Assignment 2 - Average = %f\n", finalavg);
                sum = 0;
                finalavg = 0;
                exit(0);
            }
        }
        exit(0);
    }

    if (GradTA > 0)
    {
        GradTA_2 = fork();

        if (GradTA_2 == 0)
        {
           // printf("GradTA 2 PID: %d PPID: %d\n", getpid(), getppid());

            pid_t TA_3 = fork();

            if (TA_3 == 0)
            {
                //printf("TA 3 PID: %d PPID: %d\n", getpid(), getppid());
                for (int i = 0; i < 10; i++)
                {
                    sum += grades[2][i];
                }
                finalavg = (float)sum / 10;
                printf("Assignment 3 - Average = %f\n", finalavg);
                sum = 0;
                finalavg = 0;
                exit(0);
            }

            if (TA_3 > 0)
            {
                pid_t TA_4 = fork();
                if (TA_4 == 0)
                {
                    //printf("TA 4 PID: %d PPID: %d\n", getpid(), getppid());
                    for (int i = 0; i < 10; i++)
                    {
                        sum += grades[3][i];
                    }
                    finalavg = (float)sum / 10;
                    printf("Assignment 4 - Average = %f\n", finalavg);
                    sum = 0;
                    finalavg = 0;
                    exit(0);
                }
            }
            exit(0);
        }
        if (GradTA_2 > 0)
        {
            GradTA_3 = fork();

            if (GradTA_3 == 0)
            {
           //     printf("GradTA 3 PID: %d PPID: %d\n", getpid(), getppid());

                pid_t TA_5 = fork();

                if (TA_5 == 0)
                {
                    //printf("TA 5 PID: %d PPID: %d\n", getpid(), getppid());
                    for (int i = 0; i < 10; i++)
                    {
                        sum += grades[4][i];
                    }
                    finalavg = (float)sum / 10;
                    printf("Assignment 5 - Average = %f\n", finalavg);
                    sum = 0;
                    finalavg = 0;
                    exit(0);
                }

                if (TA_5 > 0)
                {
                    pid_t TA_6 = fork();
                    if (TA_6 == 0)
                    {
                        //printf("TA 6 PID: %d PPID: %d\n", getpid(), getppid());
                        for (int i = 0; i < 10; i++)
                        {
                            sum += grades[5][i];
                        }
                        finalavg = (float)sum / 10;
                        printf("Assignment 6 - Average = %f\n", finalavg);
                        sum = 0;
                        finalavg = 0;
                        exit(0);
                    }
                }
                exit(0);
            }
        }
    }

    if (GradTA > 0 && GradTA_2 > 0 && GradTA_3 > 0)
    {
        for (int i = 0; i < 10; i++)
        {
            wait(NULL);
        }
       // printf("I am parent PID: %d\n", getpid());
    }

    return 0;
}
