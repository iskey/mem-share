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

/* the maxinum share channel */
#define MAX_NODE_NUM 10
/* the shm base key value. */
#define BASE_KEY 35

/* variant to identify channels */
static int g_chn_indx= 0;
/* work model. 0:push modle,1:pull modle. */
static int g_work_model= 0;
/* share memory address */
static void *g_shm_addr[MAX_NODE_NUM]= {0};
/* share memory keys */
static key_t g_shmkey[MAX_NODE_NUM];
/* share memory id. used by shm_uinit */
static int g_shm_id[MAX_NODE_NUM];

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

    if((0!= work_model)&& (1!= work_model)){
        printf("work model not supported!\n");
        goto err;
    }
    /* initial work model */
    g_work_model= work_model;

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
SHM_FD shm_chn_add(int max_buf_size)
{
    void *ret;

    /* check variant */
    if(max_buf_size<= 0){
        printf("max buffer size is not support!\n");
        goto err;
    }

    if(0== g_work_model)
    {
        g_shm_id[g_chn_indx]= shmget(g_shmkey[g_chn_indx], max_buf_size, 0777| IPC_CREAT);
        if(-1== g_shm_id[g_chn_indx]){
            printf("share memory get error! err:%s\n",strerror(errno));
            goto err;
        }

        ret= shmat(g_shm_id[g_chn_indx], 0, 0);
        if((void *)-1== ret){
            printf("share memory attach address error\n");
            goto err;
        }
        g_shm_addr[g_chn_indx]= ret;
        ((SHARE_BUF_NODE *)g_shm_addr[g_chn_indx])->max_buf_size= max_buf_size;
        ((SHARE_BUF_NODE *)g_shm_addr[g_chn_indx])->share_pt= ret+ sizeof(SHARE_BUF_NODE);
    }

    if(1== g_work_model){
        printf("operation is not suppored by this model!\n");
        printf("may try shm_chn_attach please!\n");
        goto err;
    }

    return g_chn_indx++;
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

    if(0== g_work_model)
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
                    mem_ret= shmat(g_shm_id[g_chn_indx], 0, 0);
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

    if(1== g_work_model){
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
SHM_FD shm_chn_attach(void)
{
    void *ret;
    int max_buf_size= 0;

    if(1== g_work_model)
    {
        g_shm_id[g_chn_indx]= shmget(g_shmkey[g_chn_indx], sizeof(SHARE_BUF_NODE), 0777| IPC_CREAT);
        if(-1== g_shm_id[g_chn_indx]){
            printf("share memory get error!\n");
            goto err;
        }

        ret= shmat(g_shm_id[g_chn_indx], 0, 0);
        if((void *)-1== ret){
            printf("share memory attach address error\n");
            goto err;
        }

        max_buf_size= ((SHARE_BUF_NODE*)ret)->max_buf_size;

        g_shm_id[g_chn_indx]= shmget(g_shmkey[g_chn_indx], max_buf_size, 0777| IPC_CREAT);
        if(-1== g_shm_id[g_chn_indx]){
            printf("share memory get error!\n");
            goto err;
        }

        ret= shmat(g_shm_id[g_chn_indx], 0, 0);
        if((void *)-1== ret){
            printf("share memory attach address error\n");
            goto err;
        }

        g_shm_addr[g_chn_indx]= ret;
        ((SHARE_BUF_NODE *)g_shm_addr[g_chn_indx])->share_pt= ret+ sizeof(SHARE_BUF_NODE);
    }

    if(0== g_work_model){
        printf("operation is not suppored by this model!\n");
        printf("may try shm_chn_add please!\n");
        goto err;
    }

    return g_chn_indx++;

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
    if((mfd< 0)&& (mfd> MAX_NODE_NUM)){
        printf("SHM_FD may be not right!\n");
        goto err;
    }
    if(((node->share_size)<0)|| ((node->share_size)> ((SHARE_BUF_NODE *)g_shm_addr[mfd])->max_buf_size)){
        printf("node buffer size is too big!\n");
        goto err;
    }

    ((SHARE_BUF_NODE *)g_shm_addr[mfd])->share_size= node->share_size;
    memcpy(((SHARE_BUF_NODE *)g_shm_addr[mfd])->share_pt, node->share_pt, node->share_size);

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
    node->share_size= ((SHARE_BUF_NODE*)g_shm_addr[mfd])->share_size;
    node->share_pt= ((SHARE_BUF_NODE*)g_shm_addr[mfd])->share_pt;
    node->max_buf_size= ((SHARE_BUF_NODE*)g_shm_addr[mfd])->max_buf_size;

    return 0;

err:
    return -1;
}
