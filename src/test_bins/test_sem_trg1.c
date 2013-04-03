#include "sem-trigger.h"
#include "stdio.h"

#define KEY_ID 11

int main()
{
    pid_t pid;

    long times= 0;

    int ret;
    SEM_FD fd;
    sem_trigger_init(TRG_SERVER_MODEL);
    fd= sem_trigger_add(KEY_ID);

    srand((int)time(0));

    while(1)
    {
        sem_trigger_lock(fd);
        printf("pid[%d] get consumer signal times= %d\n",getpid(), times);
        if(times> 30000) times= 1;
        usleep(rand()%1000);
        printf("pid[%d] get consumer signal times= %d end\n",getpid(), times++);
        sem_trigger_unlock(fd);
    }
    return 0;

//    free_sem(semid);
    printf("sem freed\n");
    return 0;
}
