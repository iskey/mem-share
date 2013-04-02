#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <errno.h>

#include "base-sem.h"
#include "sem-trigger.h"

#define TRG_SERVER_MODEL 0
#define TRG_CLIENT_MODEL 1
#define TRG_MODEL_NOT_SUPPORT 0xff

#define TRG_ID_0 0
#define TRG_ID_1 1

/* 0:trigger 1, 1:trigger 2 */
static int g_work_model= TRG_MODEL_NOT_SUPPORT;

int sem_trigger_init(int work_model)
{
    if((TRG_CLIENT_MODEL!= work_model)&& (TRG_SERVER_MODEL!= work_model)){
        printf("work_model not supported!\n");
        goto err;
    }
    else{
        g_work_model= work_model;
    }

    return 0;

err:
    return -1;
}
int sem_trigger_check(void)
{
    if(TRG_MODEL_NOT_SUPPORT== g_work_model){
        printf("You must initial this sem_trigger_model.");
        return -1;
    }
    else {
        return 0;
    }
}
static int sem_trigger_create(int key)
{
    int sem_id;
    sem_id= create_sem(key, 2);
    if(-1== sem_id){
        printf("create sem error: %s\n",strerror(errno));
        goto err;
    }
    set_sem_value(sem_id, TRG_ID_0, 1);
    set_sem_value(sem_id, TRG_ID_1, 0);

    return sem_id;

err:
    return -1;
}
static int sem_trigger_attach(int key)
{
    int sem_id;
    sem_id= create_sem(key, 2);
    if(-1== sem_id){
        printf("create sem error: %s\n",strerror(errno));
        goto err;
    }

    return sem_id;

err:
    return -1;

}
SEM_FD sem_trigger_add(int key)
{
    int ret;
    if(-1== sem_trigger_check()){
        goto err;
    }

    if(TRG_SERVER_MODEL== g_work_model){
        ret= sem_trigger_create(key);
        if(-1== ret){
            printf("sem trigger create error: %s", strerror(errno));
            goto err;
        }
    }
    else {
        ret= sem_trigger_attach(key);
        if(-1== ret){
            printf("sem trigger attach error: %s", strerror(errno));
        }

    }

    printf("trigger sem id= %d\n", ret);
    return ret;

err:
    return -1;
}

static int sem_trigger1_lock(int fd)
{
    int ret;
    ret= lock_sem(fd, TRG_ID_0);
    if(-1== ret){
        printf("lock_sem error: %s\n", strerror(errno));
        goto err;
    }

    return 0;

err:
    return -1;
}
static int sem_trigger2_lock(int fd)
{
    int ret;
    ret= lock_sem(fd, TRG_ID_1);
    if(-1== ret){
        printf("lock_sem error: %s\n", strerror(errno));
        goto err;
    }

    return 0;

err:
    return -1;
}
int sem_trigger_lock(SEM_FD sem_fd)
{
    int ret;
    if(-1== sem_trigger_check()){
        goto err;
    }

    if(TRG_SERVER_MODEL== g_work_model){
        if(-1== sem_trigger1_lock(sem_fd)){
            printf("sem trigger lock error: %s\n", strerror(errno));
            goto err;
        }
    }
    else {
        if(-1== sem_trigger2_lock(sem_fd)){
            printf("sem trigger lock error: %s\n", strerror(errno));
        }
    }

    return 0;

err:
    return -1;

}
int sem_trigger_unlock(SEM_FD sem_fd)
{
    int ret;
    if(-1== sem_trigger_check()){
        goto err;
    }

    if(TRG_SERVER_MODEL== g_work_model){
        set_sem_value(sem_fd, TRG_ID_1, 1);
        if(-1== ret){
            goto err;
        }
    }
    else {
        set_sem_value(sem_fd, TRG_ID_0, 1);
        if(-1== ret){
                goto err;
        }
    }

    return 0;

err:
    printf("sem trigger unlock error: %s\n", strerror(errno));
    return -1;
}
