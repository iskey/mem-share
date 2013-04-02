#include "sem-trigger.h"
#include "stdio.h"

#define KEY_ID 11

int main()
{
    pid_t pid;

    long times= 0;

    int ret;
    SEM_FD fd;
    sem_trigger_init(1);
    fd= sem_trigger_add(KEY_ID);
    srand((int)time(0)/3);

    while(1)
    {
        sem_trigger_lock(fd);
        printf("pid[%d] get producter signal times= %d\n", getpid(), times);
        usleep(rand()%1000);
        if(times> 30000) times= 1;
        printf("pid[%d] get producter signal times= %d end\n", getpid(), times++);
        sem_trigger_unlock(fd);
    }

//    free_sem(semid);
    printf("sem freed\n");
    return 0;
}
