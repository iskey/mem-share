/******************************************************************************

           This is Iskey.

 ******************************************************************************/
/**
 * @file    mem-share.c
 * @brief   mem-share functions
 * @author  iskey@outlook.com
 * @version Initial Draft
 * @note    none
 * @date    2013/3/19
 */
/******************************************************************************
 *  Function List :
 *              shm_chn_add
 *              shm_chn_attach
 *              shm_init
 *              shm_pull
 *              shm_push
 *              shm_uinit
 *  History       :
 *  1.Date        : 2013/3/19
 *    Author      : iskey
 *    Modification: Created file
 *
******************************************************************************/

/** external variables */

/** external routine prototypes */

/** internal routine prototypes */

/** project-wide global variables */

/** module-wide global variables */

/** constants */

/** macros */

/** routines' implementations */

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>

#include "mem-share.h"
#include "sem-trigger.h"

/* the maxinum share channel */
#define MAX_NODE_NUM 10
/* the shm base key value. */
#define BASE_KEY 35
/* work model. 0:push modle,1:pull modle. */
static int g_work_model= 0;
/* share memory address */
static void *g_shm_addr[MAX_NODE_NUM]= {0};
/* share memory keys */
static key_t g_shmkey[MAX_NODE_NUM];
/* share memory id. used by shm_uinit */
static int g_shm_id[MAX_NODE_NUM];
/* sem trigger id */
static SEM_FD g_sem_fd[MAX_NODE_NUM];

/**
 * @brief init share memory.
 * @param [in] int work_model  : 0:push model, 1:pull model.
 * @param [out] None
 * @return
 * @note
 *  Calls      :
 *  Called By  :
 *
 *  History        :
 *  1.Date         : 2013/3/19
 *    Author       : iskey
 *    Modification : Created function
 */
int shm_init(int work_model)
{
    int i=0;
    int ret;
    int trg_model;

    /* check work model */
    if(SHM_PUSH_MODEL== work_model){
        trg_model= TRG_SERVER_MODEL;
    }
    else if(SHM_PULL_MODEL== work_model){
        trg_model= TRG_CLIENT_MODEL;
    }
    else {
        printf("work model not supported!\n");
        goto err;
    }

    /* initial work model */
    g_work_model= work_model;
    /* initial sem trigger */
    ret= sem_trigger_init(trg_model);
    if(-1== ret){
        printf("sem trigger error: %s\n", strerror(errno));
        goto err;
    }
    /* initial shm key id. */
    for(i= 0; i< MAX_NODE_NUM; i++){
        g_shmkey[i]= ftok(".", BASE_KEY+ i);
    }

err:
    return -1;
}
/* add one transfer channel. */
/**
 * @brief add one memory share channel.
 * @param [in] int max_buf_size  : the maxinum size of the share memory.
 * @param [out] None
 * @return
 * @note
 *  Calls      :
 *  Called By  :
 *
 *  History        :
 *  1.Date         : 2013/3/19
 *    Author       : iskey
 *    Modification : Created function
 */
SHM_FD shm_chn_add(int chn_indx, int max_buf_size)
{
    void *ret;

    /* check variant */
    if(max_buf_size<= 0){
        printf("max buffer size is not support!\n");
        goto err;
    }

    if(SHM_PUSH_MODEL== g_work_model)//push model
    {
        g_sem_fd[chn_indx]= sem_trigger_add(chn_indx);
        printf("sem trigger %d added!\n", g_sem_fd[chn_indx]);

        g_shm_id[chn_indx]= shmget(g_shmkey[chn_indx], max_buf_size, 0777| IPC_CREAT);
        if(-1== g_shm_id[chn_indx]){
            printf("share memory get error! err:%s\n",strerror(errno));
            goto err;
        }

        ret= shmat(g_shm_id[chn_indx], 0, 0);
        if((void *)-1== ret){
            printf("share memory attach address error\n");
            goto err;
        }
        g_shm_addr[chn_indx]= ret;
        ((SHARE_BUF_NODE *)g_shm_addr[chn_indx])->max_buf_size= max_buf_size;
        ((SHARE_BUF_NODE *)g_shm_addr[chn_indx])->share_pt= ret+ sizeof(SHARE_BUF_NODE);
    }

    if(SHM_PULL_MODEL== g_work_model){//pull model
        printf("operation is not suppored by this model!\n");
        printf("may try shm_chn_attach please!\n");
        goto err;
    }

    return chn_indx++;
err:
    return -1;
}
/**
 * @brief delete all share channels in the memory.
 * @param [in] None
 * @param [out] None
 * @return
 * @note
 *  Calls      :
 *  Called By  :
 *
 *  History        :
 *  1.Date         : 2013/3/19
 *    Author       : iskey
 *    Modification : Created function
 */
int shm_uinit()
{
    int ret, tmp_indx;
    struct shmid_ds buf;

    if(SHM_PUSH_MODEL== g_work_model)
    {
        for(tmp_indx=0; tmp_indx< MAX_NODE_NUM; tmp_indx++)
        {
            void *mem_ret;
            printf("tmp_index = %d\n",tmp_indx);
            g_shm_id[tmp_indx]= shmget(g_shmkey[tmp_indx], sizeof(SHARE_BUF_NODE), 0777| IPC_CREAT| IPC_EXCL);
            if(-1== g_shm_id[tmp_indx]){
                if(errno== EEXIST){
//                    printf("share memory get error!\n");
//                    printf("err: %s\n",strerror(errno));
                    g_shm_id[tmp_indx]= shmget(g_shmkey[tmp_indx], sizeof(SHARE_BUF_NODE), 0777| IPC_CREAT);
                    mem_ret= shmat(g_shm_id[tmp_indx], 0, 0);
                    if((void *)-1== mem_ret){
                        printf("share memory attach address error\n");
                    }

                    ret= shmctl(g_shm_id[tmp_indx], IPC_RMID, 0);
                    if(-1== ret){
                        printf("delete share memory false!\n");
                        printf("err: %s\n", strerror(errno));
                        continue;
                    }

                    if(-1== shmdt(mem_ret)){
                        printf("detach share memory false!\n");
                        printf("err: %s\n", strerror(errno));
                        continue;
                    }
                }
                else {
                    goto err;
                }
            }
        }
    }

    if(SHM_PULL_MODEL== g_work_model){
        printf("operation is not suppored by this model!\n");
        goto err;
    }

err:
    return -1;
}
/**
 * @brief attach one share channel.
 * @param [in] void
 * @param [out] None
 * @return
 * @note
 *  Calls      :
 *  Called By  : pullers.
 *
 *  History        :
 *  1.Date         : 2013/3/19
 *    Author       : iskey
 *    Modification : Created function
 */
SHM_FD shm_chn_attach(int chn_indx)
{
    void *shm_addr_pt;
    int ret;
    int max_buf_size= 0;

    if(SHM_PULL_MODEL== g_work_model)//pull model
    {
        g_sem_fd[chn_indx]= sem_trigger_add(chn_indx);
        printf("sem trigger %d attached!\n", g_sem_fd[chn_indx]);

        /////********* get SHARE_BUF_NODE structure *************/
        /* we donot konw how many byte to get, so we get SHARE_BUF_NODE
           sturcture first */
        g_shm_id[chn_indx]= shmget(g_shmkey[chn_indx], sizeof(SHARE_BUF_NODE), 0);
        if(-1== g_shm_id[chn_indx]){
            printf("share memory may not created by pusher yet. you need to try this later.\n");
            goto err;
        }
        /* map the memory address */
        shm_addr_pt= shmat(g_shm_id[chn_indx], 0, 0);
        if((void *)-1== shm_addr_pt){
            printf("share memory attach address error.\n");
            goto err;
        }
        /* get the maximum size of the share buffer */
        max_buf_size= ((SHARE_BUF_NODE*)shm_addr_pt)->max_buf_size;
        /////********* end get SHARE_BUF_NODE structure **********/

        /////********* get the real structure *************/
        g_shm_id[chn_indx]= shmget(g_shmkey[chn_indx], max_buf_size, 0);
        if(-1== g_shm_id[chn_indx]){
            printf("share memory get error!\n");
            goto err;
        }

        shm_addr_pt= shmat(g_shm_id[chn_indx], 0, 0);
        if((void *)-1== shm_addr_pt){
            printf("share memory attach address error\n");
            goto err;
        }
        /////********* end get the real structure *********/
        g_shm_addr[chn_indx]= shm_addr_pt;
        ((SHARE_BUF_NODE *)g_shm_addr[chn_indx])->share_pt= shm_addr_pt+ sizeof(SHARE_BUF_NODE);
    }

    if(SHM_PUSH_MODEL== g_work_model){//push model
        printf("operation is not suppored by this model!\n");
        printf("may try shm_chn_add please!\n");
        goto err;
    }

    return chn_indx;

err:
    return -1;
}
/**
 * @brief push block of data to share channel.
 * @param [in] SHM_FD mfd  : handler of the share channel.
 * @param [in] SHARE_BUF_NODE *node  : data block structure.
 * @param [out] None
 * @return
 * @note
 *  Calls      :
 *  Called By  :
 *
 *  History        :
 *  1.Date         : 2013/3/19
 *    Author       : iskey
 *    Modification : Created function
 */
int shm_push(SHM_FD mfd, SHARE_BUF_NODE *node)
{
    if(-1== sem_trigger_lock(g_sem_fd[mfd])){
        printf("sem trigger lock error! shm push operation may be not work properly");
    }

    if((mfd< 0)&& (mfd> MAX_NODE_NUM)){
        printf("SHM_FD may be not right!\n");
        goto err;
    }
    if(((node->share_size)<0)|| ((node->share_size)> ((SHARE_BUF_NODE *)g_shm_addr[mfd])->max_buf_size)){
        printf("node buffer size is too big!\n");
        goto err;
    }

    ((SHARE_BUF_NODE *)g_shm_addr[mfd])->share_size= node->share_size;
    memcpy(g_shm_addr[mfd]+ sizeof(SHARE_BUF_NODE), node->share_pt, node->share_size);

    shm_release(mfd);

    return 0;

err:
    return -1;
}
/**
 * @brief pull block of data to share channel
 * @param [in] SHM_FD mfd  : handler of the share channel
 * @param [in] SHARE_BUF_NODE *node  : data block structure.
 * @param [out] None
 * @return
 * @note
 *  Calls      :
 *  Called By  :
 *
 *  History        :
 *  1.Date         : 2013/3/19
 *    Author       : iskey
 *    Modification : Created function
 */
int shm_pull(SHM_FD mfd, SHARE_BUF_NODE *node)
{
    if(-1== sem_trigger_lock(g_sem_fd[mfd])){
        printf("sem trigger lock error! shm push operation may be not work properly");
    }

    node->share_size= ((SHARE_BUF_NODE*)g_shm_addr[mfd])->share_size;
    node->share_pt= g_shm_addr[mfd]+ sizeof(SHARE_BUF_NODE);
    node->max_buf_size= ((SHARE_BUF_NODE*)g_shm_addr[mfd])->max_buf_size;

    return mfd;

err:
    return -1;
}
int shm_release(int fd)
{
    SHM_FD mfd= fd;
    return(sem_trigger_unlock(g_sem_fd[mfd]));
}

