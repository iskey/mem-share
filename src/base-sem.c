#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* to avoid implicit declaration warnings */
extern int semtimedop(int semid, struct sembuf *sops, unsigned nsops,struct timespec *timeout);

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* create semaphore */
int create_sem(int key_id, int count)
{
    key_t key;
//    key= key_id;
    key= ftok(".", key_id);

    int semid;
    semid= semget(key, count, 0666| IPC_CREAT);
    if(-1== semid){
        printf("create sem error: %s, semid= %d\n", strerror(errno), semid);
    }

    return semid;
}
/* create semaphore */
int attach_sem(int key_id, int count)
{
    key_t key;
//    key= key_id;
    key= ftok(".", key_id);

    int semid;
    semid= semget(key, count, 0);
    if(-1== semid){
        printf("create sem error: %s, semid= %d\n", strerror(errno), semid);
    }

    return semid;
}
/* delete semaphore */
int free_sem(int semid)
{
    union semun ignored_arg;
    return semctl(semid, 0, IPC_RMID, ignored_arg);
}
/* set semaphore value */
int set_sem_value(int semid, int index, int value)
{
    union semun arg;
    arg.val= value;
    int ret;
    ret= semctl(semid, index, SETVAL, arg);
    if(-1== ret){
        printf("set_sem_value error: %s\n",strerror(errno));
    }

    return ret;
}
/* get semaphore value */
int get_sem_value(int semid, int index)
{
    int ret= semctl(semid, index, GETVAL, 0);
    if(-1== ret){
        printf("get_sem_value error: %s\n",strerror(errno));
    }

    return ret;
}
/* block when semid[index] is negative and ms has not timeout. */
int try_lock_sem(int semid, int index, int ms)
{
    int ret;
    struct sembuf op;
    op.sem_num= index;
    op.sem_op= -1;

    if(ms< 1){
        op.sem_flg= IPC_NOWAIT;
        ret= semop(semid, &op, 1);
        if(-1== ret){
            printf("try_lock_sem error: %s\n",strerror(errno));
        }
    }
    else
    {
        /* to avoid unwanted exited */
        op.sem_flg= SEM_UNDO;
        struct timespec timeout;
        timeout.tv_sec= ms/ 1000;
        timeout.tv_nsec= (ms% 1000L)* 1000000L;
        ret= semtimedop(semid, &op, 1, &timeout);
        if(-1== ret){
            printf("try_lock_sem error: %s\n",strerror(errno));
        }
    }

//    if(-1== ret){
//        if(errno== EAGAIN) return EAGAIN;
//    }
    return ret;
}
/* block when sem[index] is negative, until it become plus */
int lock_sem(int semid, int index)
{
    struct sembuf op;
    op.sem_num= index;
    op.sem_op= -1;
    op.sem_flg= SEM_UNDO;
    int ret;
    ret= semop(semid, &op, 1);
    if(-1== ret){
        printf("pid[%d] lock_sem error: %s\n", getpid(), strerror(errno));
    }
    return ret;
}
/* unlock semaphore */
int unlock_sem(int semid, int index)
{
    struct sembuf op;
    op.sem_num= index;
    op.sem_op= 1;
    op.sem_flg= SEM_UNDO;

    int ret;
    ret= semop(semid, &op, 1);
    if(-1== ret){
        printf("pid[%d] unlock_sem error: %s, semid=%d index=%d\n", getpid(), strerror(errno), semid, index);
    }

    return ret;
}
