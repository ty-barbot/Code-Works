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

sem_t *semaphore1;
sem_t *semaphore2;
int pipe1[2];
int pipe2[2];
int shm_fd;

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        printf("invalid args <input filename> <output filename> \n");
				printf("look at the compile file the the directory.\n");
        exit(EXIT_FAILURE);
    }
    

		//create semahpore1 
    semaphore1 = sem_open("/prog0_sem_1", O_CREAT, 0600, 1);
    if(semaphore1 == SEM_FAILED)
    {
        printf("semaphore1 error\n");
        exit(1);
    }

		//create semaphore2
    semaphore2 = sem_open("/semaphore2", O_CREAT, 0600, 1);
    if(semaphore2 == SEM_FAILED)
    {
        printf("semaphore2 error\n");
        exit(1);
    }

    if(pipe(pipe1) == -1)
    {
        printf("Pipe1 error\n");
        exit(1);
    }
    
    if(pipe(pipe2) == -1)
    {
        printf("Pipe2 error\n");
        exit(1);
    }

		char pipe1_0[10];//read
		char pipe1_1[10];//write

		char pipe2_0[10];//read
		char pipe2_1[10];//write

    sprintf(pipe1_0, "%d", pipe1[0]);
    sprintf(pipe1_1, "%d", pipe1[1]);

    sprintf(pipe2_0, "%d", pipe2[0]);
    sprintf(pipe2_1, "%d", pipe2[1]);

		//create the sharded memory page
    shm_fd = shm_open("TestMem", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, 9);


		//create a multi demenisional array
		//each column is the program the args are being sent too
		// each row is the argument itself
    char* args[3][7] = {
        // prog name , in file,        sem 1 name,        pipe1
        {"./program1", argv[1],        "/prog0_sem_1",    pipe1_1,       NULL, NULL, NULL},

        // prog name,  sem 1 name,     sem 2 name,       pipe1,       pipe 2,   shared mem id
        {"./program2", "/prog0_sem_1", "/semaphore2",    pipe1_0,     pipe2_1,  "TestMem", NULL},

        // prog name,  sem 2 name,    outfile, pipe 2,  2+3 shared
        {"./program3", "/semaphore2", argv[2], pipe2_0, "TestMem",    NULL, NULL}
    };

    pid_t pids[3];
    int i = 0;

    if((pids[0] = fork()) < 0)
    {
        perror("fork 1");
        exit(1);
    }
    else if(pids[0] == 0)
    {
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);
        //char* args[4] = {"program1", }
        execv(args[0][0], args[0]);
        perror("Child 1 execv failed\n");
    }

    if((pids[1] = fork()) < 0)
    {
        perror("fork 2");
        exit(1);
    }
    else if(pids[1] == 0)
    {
        close(pipe1[1]);
        close(pipe2[0]);
        execv(args[1][0], args[1]);
    }

    if((pids[2] = fork()) < 0)
    {
        perror("fork 3");
        exit(1);
    }

		//clsose pipes
    if(pids[2] == 0)
    {
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);
        execv(args[2][0], args[2]);
    }

    int status;
    waitpid(pids[0], &status, 0);
    waitpid(pids[1], &status, 0);
    waitpid(pids[2], &status, 0);

    sem_unlink("/semaphore2");
    
    return 0;
}
