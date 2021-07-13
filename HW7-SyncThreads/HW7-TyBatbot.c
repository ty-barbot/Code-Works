/*
* I had quite a hard time detaching threads in C++
* and I think the assinment is much too hard to 
* implement in C++ so I got some help with the C code
* 
* I Assume the file format as +100 and -100 
* as a positve 100 and negetive 100 cause 
* all i do is all them and when you add a 
* negative you subtract.
*
*  gcc -o Hw7 Hw7.c -pthread
* ./Hw7
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct getThreads
{
	char *File;
	char *monsterName;
};
float BALANCE = 0;
pthread_mutex_t m;

void *readTheFile(void *arg)
{
	struct getThreads *monsterArray_poogers = (struct getThreads *)arg;
	char *File = monsterArray_poogers->File;
	FILE *file = fopen(File, "r");
	if(!file)
	{
		printf("bad file, throwing file error \n");
		exit(0);
	}
	while(1)
	{
		char readStuff[100];
		fscanf(file, "%s\n", readStuff);
		if(feof(file))
		{
			break;
		}
		if(readStuff[0] == 'R' || readStuff[0] == 'r')
		{
			float temp = 0;
			while(pthread_mutex_trylock(&m)); // knock until you can enter
			{
				//dont need to do anything (normally the condiational variable)
			}
			while(1)
			{
			fscanf(file, "%s\n", readStuff);
			if(feof(file))
			{
				BALANCE += temp;
				//Account balance after thread *thread_name_here* is $xxx
				printf("Account balance after thread %s is $%.2f \n", monsterArray_poogers->monsterName, BALANCE);
				pthread_mutex_unlock(&m);
				break;
			}
			if(readStuff[0] == 'W' || readStuff[0] == 'w' )
			{
				BALANCE += temp;
				//Account balance after thread *thread_name_here* is $xxx
				printf("account balance after thread %s is $%5.2f\n", monsterArray_poogers->monsterName, BALANCE);
				pthread_mutex_unlock(&m);
				break;
			}
temp += strtof(readStuff, NULL); 
			}
		}
	}
}

int main(void)
{
	#ifdef TEST
	char* File[] = {"Vlad.in","Frank.in","Bigfoot.in","Casper.in","Gomez.in"};
	int i;

	for(i = 0; i < 5; i++)
	{
		FILE *file = fopen(File[i], "r");
		while(1)
		{
			char readStuff[100];
			fscanf(file, "%s\n", readStuff);
			if(feof(file)) 
			{
				break;
			}
			if(readStuff[0] == '+' || readStuff[0] == '-')
			{
				BALANCE += strtof(readStuff, NULL);
				//adding a negative is the same a subtracting, so i hope there is no space
			}
		}
		fclose(file);
		file = NULL;
	}
	#endif
  pthread_t monsterArray[5];
  struct getThreads structAarray[5];

  structAarray[0].File = "Vlad.in";
	structAarray[0].monsterName = "Vlad";

  structAarray[1].File = "Frank.in";
	structAarray[1].monsterName = "Frank";

  structAarray[2].File = "Bigfoot.in";
	structAarray[2].monsterName = "Bigfoot";

  structAarray[3].File = "Casper.in";
	structAarray[3].monsterName = "Casper";

  structAarray[4].File = "Gomez.in";
	structAarray[4].monsterName = "Gomez";

  pthread_create(&monsterArray[0], NULL, readTheFile, (void *)&structAarray[0]);
  pthread_create(&monsterArray[1], NULL, readTheFile, (void *)&structAarray[2]);
  pthread_create(&monsterArray[2], NULL, readTheFile, (void *)&structAarray[1]);
  pthread_create(&monsterArray[3], NULL, readTheFile, (void *)&structAarray[3]);
  pthread_create(&monsterArray[4], NULL, readTheFile, (void *)&structAarray[4]);

	pthread_join(monsterArray[0], NULL);
  pthread_join(monsterArray[1], NULL);
  pthread_join(monsterArray[2], NULL);
  pthread_join(monsterArray[3], NULL);
  pthread_join(monsterArray[4], NULL);
}
