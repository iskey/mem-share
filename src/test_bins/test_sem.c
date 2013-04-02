#include "base-sem.h"
#include "stdio.h"

#define KEY_ID 11

int main()
{
    pid_t pid;
    int semid;
    semid= create_sem(KEY_ID, 2);
    set_sem_value(semid, 0, 1);
    set_sem_value(semid, 1, 0);

    long times= 0;
    long times1= 0;

    pid= fork();
    if(pid== 0)
    {
        int ret;

        srand((int)time(0));

        while(1)
        {
            ret= lock_sem(semid, 1);
            if(-1== ret){
                int t= get_sem_value(semid, 1);
                printf("pid[%d] current semid index[%d] value is %d\n", getpid(), 1, t);
            }
            printf("pid[%d] get consumer signal times= %d\n",getpid(), times1++);
            if(times1> 30000) times1= 1;

            ret= set_sem_value(semid, 0, 1);
            if(-1== ret){
                int t= get_sem_value(semid, 0);
                printf("pid[%d] current semid index[%d] value is %d\n", getpid(), 0, t);
            }
        }
        return 0;
    }

    while(1)
    {
        int ret;

        srand((int)time(0)/3);

        ret= lock_sem(semid, 0);
        if(-1== ret){
            int t= get_sem_value(semid, 0);
            printf("pid[%d] current semid index[%d] value is %d\n", getpid(), 0, t);
        }
        printf("pid[%d] get producter signal times= %d\n", getpid(), times++);
        if(times> 30000) times= 1;
        ret= set_sem_value(semid, 1, 1);
        if(-1== ret){
            int t= get_sem_value(semid, 1);
            printf("pid[%d] current semid index[%d] value is %d\n", getpid(), 1, t);
        }
    }

    free_sem(semid);
    printf("sem freed\n");
    return 0;
}
