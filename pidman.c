#define MIN_PID 300
#define MAX_PID 5000
#define THREADS 100

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int pid_map[MAX_PID - MIN_PID];

int allocate_map()
{
	int i;

	for(i = 0; i < MAX_PID - MIN_PID + 1; i++)
		pid_map[i] = 0;

	if(i == 4701)
		return 1;
	else
		return -1;
}

int allocate_pid()
{
	int i;

	for(i = 0; i < MAX_PID - MIN_PID + 1; i++)
	{
		if(!pid_map[i])
		{
			pid_map[i] = 1;
			printf("Allocated %d\n", i + MIN_PID);
			return i + MIN_PID;
		}
	}
}

void release_pid(int pid)
{
	pid_map[pid - 300] = 0;
	printf("Released %d\n", pid);
}

void *thread_exec(void *param)
{
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);

	pthread_mutex_lock(&mutex);
	int pid = allocate_pid();
	pthread_mutex_unlock(&mutex);

	sleep(rand() % 5 + 1);

	pthread_mutex_lock(&mutex);
	release_pid(pid);
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

int main()
{
	int i;
	pthread_t tids[THREADS];
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	allocate_map();
	srand(time(NULL));

	for(i = 0; i < THREADS + 1; i++)
		pthread_create(&tids[i], &attr, thread_exec, NULL);
	
	for(i = 0; i < THREADS + 1; i++)
	{
		pthread_join(tids[i], NULL);
		wait(NULL);
	}
}
