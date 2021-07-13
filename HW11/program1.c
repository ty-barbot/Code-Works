#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

sem_t *semaphoreTest;
int pipe1_write;

int main(int argc, char** argv)
{
    if(argc == 4)
    {
        semaphoreTest = sem_open(argv[2], 0);
        if(semaphoreTest == SEM_FAILED)
        {
            printf("semaphoreTest error\n");
            exit(1);
        }
        pipe1_write = atoi(argv[3]);
        int i = 0;
        sem_getvalue(semaphoreTest, &i);
    }
    else
    {
        printf("Program1 error");
        exit(1);
    }
    
    FILE *fptr = fopen(argv[1], "r");
    if(!fptr)
    {
        printf("file error\n.");
        exit(1);
    }

    char input_str[20];
    int i = 0;
    while(1)
    {
        fscanf(fptr, "%s", input_str);
        sem_getvalue(semaphoreTest, &i);
        while(i == 2)
        {
            sem_getvalue(semaphoreTest, &i);

        }

        dprintf(pipe1_write, "%s`", input_str);
        sem_post(semaphoreTest);

        if(feof(fptr)) break;
    }
    fclose(fptr);


    sem_getvalue(semaphoreTest, &i);
    while(i == 2)
    {
        sem_getvalue(semaphoreTest, &i);
    }
    dprintf(pipe1_write, "EOF`");
    sem_post(semaphoreTest);

    sem_close(semaphoreTest);
    close(pipe1_write);

    return 0;
}
