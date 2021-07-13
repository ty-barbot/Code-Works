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

// only need to read pipe2
sem_t *semaphore2;

int pipe2_read;
void *voidPointer;
char *temp;
char *typeCount;
char c;
char string[100];

int main(int argc, char** argv)
{
    if(argc == 5)
    {
        semaphore2 = sem_open(argv[1], 0);
        if(semaphore2 == SEM_FAILED)
        {
            printf("semahpore2 error \n");
            exit(1);
        }
				//help from reading the pipe
        pipe2_read = atoi(argv[3]);
        int shm_fd = shm_open(argv[4], O_RDONLY, 0666);
        voidPointer = mmap(0, 8, PROT_READ, MAP_SHARED, shm_fd, 0);
    }
    else 
    {
        printf("args error????\n");
        exit(1);
    }

    FILE *file = fopen(argv[2], "w");

    int i = 0;
    while(1)
    {   
        sem_getvalue(semaphore2, &i);
        while(i == 1)
        {
            sem_getvalue(semaphore2, &i);
        }
        read(pipe2_read, &c,1);
        i = 0;
        while(c != '`')
        {
            string[i] = c;
            read(pipe2_read, & c, 1);
            i++;
        }
        string[i] = '\0'; //terminating char for a flag
        sem_wait(semaphore2);
        if(!strcmp(string, "EOF"))
        {
            break;
        }
        fprintf(file, "%s ", string);
    }

    fclose(file);
    close(pipe2_read);

		//have to allocate memory for a shared file.
    temp = malloc(sizeof(char) * 9);
    memcpy(temp, (char*)voidPointer, 9);

		//print type
    typeCount = strtok(temp, " ");
    printf("Type 1: %d\n", atoi(typeCount));
    typeCount = strtok(NULL, " ");
    printf("Type 2: %d\n", atoi(typeCount));

    return 0;
}
