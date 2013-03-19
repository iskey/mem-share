/******************************************************************************

           This is Iskey.

 ******************************************************************************/
/**
 * @file    test_push.c
 * @brief   test mem-share. work in push model.
 * @author  iskey@outlook.com
 * @version Initial Draft
 * @note    none
 * @date    2013/3/19
 */
/******************************************************************************
 *  Function List :
 *              main
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
#include <stdlib.h>

#include "mem-share.h"

void main()
{
    IPC_FD fd,fd1;

    shm_init(0);
    fd= shm_chn_add(800000);
    if(-1== fd){
        printf("shm channel add error!\n");
        return;
    }
    fd1= shm_chn_add(2048);
    if(-1== fd1){
        printf("shm channel add error!\n");
        return;
    }
    SHARE_BUF_NODE *tmp;
    tmp= malloc(sizeof(SHARE_BUF_NODE));

    tmp->share_size= sizeof("WWffffffffffffffffWXXXJKOJKOO");
    tmp->share_pt= "WWffffffffffffffffWXXXJKOJKOO";
    shm_push(fd, tmp);
    printf("share channel %d is pushed successfully by share size= %d\n",fd,tmp->share_size);

    tmp->share_size= sizeof("yyyyyyyyyyyyycccccccccc");
    tmp->share_pt= "yyyyyyyyyyyyycccccccccc";
    printf("share channel %d is pushed successfully by share size= %d\n",fd1,tmp->share_size);
    shm_push(fd1, tmp);
}
