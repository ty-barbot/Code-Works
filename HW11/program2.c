#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h> // still not sure what this one is

sem_t *semaphore1;
sem_t *semaphore2;

int pipe1_count;
int pipe2_count;
int vowelInt;
int conInt;
char term = '\0';

void *voidPointer;


void pigLatin(char string[])
{
    char c = string[0];
    if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
    {
        vowelInt++;
        int i = 0;
        term = '\0';
        while(string[i] != '\0')
        {
            if(string[i] == '.' || string[i] == ',')
            {
                term = string[i];
                break;
            }
            i++;
        }

        string[i] = 'r'; i++;
        string[i] = 'a'; i++;
        string[i] = 'y'; i++;
        if(term != '\0')
        {
            string[i] = term; i++;
        }
        string[i] = '\0';

        i = 0;
        sem_getvalue(semaphore2, &i);
        while(i == 2)
        {
            sem_getvalue(semaphore2, &i);
        }
        dprintf(pipe2_count, "%s`",string);
        sem_post(semaphore2);
        return;
    }
    else
    {
        conInt++;
        char first = string[0];
        int i = 0;
        while(string[i + 1] != '\0')
        {
            string[i] = string[i + 1];
            i++;
        }
        char term = '\0';
        if(string[i] == '.' || string[i] == ',')
        {
            term = string[i];
            i--;
        }  
        string[i] = first; i++;
        string[i] = 'a'; i++;
        string[i] = 'y'; i++;
        
        if(term != '\0')
        {
            string[i] = term; i++;
        }
        string[i] = '\0';

        i = 0;
        sem_getvalue(semaphore2, &i);
        while(i == 2)
        {
            sem_getvalue(semaphore2, &i);
        }
        dprintf(pipe2_count, "%s`", string);
        sem_post(semaphore2);
        return;
    }
    
}

int main(int argc, char** argv)
{
    vowelInt = 0; 
		conInt = 0;
    if(argc == 6)
    {
        semaphore1 = sem_open(argv[1], 0);
        if(semaphore1 == SEM_FAILED)
        {
            printf("semaphorep2\n");
            exit(1);
        }

        semaphore2 = sem_open(argv[2], 0);
        if(semaphore2 == SEM_FAILED)
        {
            printf("semahporep2\n");
            exit(1);
        }
        
        pipe1_count = atoi(argv[3]);
        pipe2_count = atoi(argv[4]);

        int shm_fd = shm_open(argv[5], O_RDWR, 0666);
        voidPointer = mmap(0, 9, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    }
		else {printf("TEST");}

    char read_c;
    char read_str[100];
    int i = 0;
    while(1)
    {   
        sem_getvalue(semaphore1, &i);
        while(i == 1)
        {
            sem_getvalue(semaphore1, &i);
        }
        read(pipe1_count, &read_c, 1);
        int j = 0;
        while(read_c != '`')
        {
            read_str[j] = read_c;
            read(pipe1_count, &read_c, 1);
            j++;
        }
        read_str[j] = '\0';
        sem_wait(semaphore1);
        
        if(!strcmp(read_str, "EOF"))
        {
            break;
        }
        pigLatin(read_str);
    }

    sem_getvalue(semaphore2, &i);
    while(i == 2)
    {
        sem_getvalue(semaphore2, &i);
    }
    dprintf(pipe2_count, "EOF`");
    sprintf(voidPointer, "%d %d", vowelInt, conInt);
    sem_post(semaphore2);

    sem_close(semaphore1);
    sem_close(semaphore2);

    sem_unlink(argv[1]);
    close(pipe1_count);
    close(pipe2_count);
    return 0;
}
