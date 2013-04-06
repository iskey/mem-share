/******************************************************************************

           This is Iskey.

 ******************************************************************************/
/**
 * @file    test_pull.c
 * @brief   test mem-share. work in pull model
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
#include <string.h>
#include "mem-share.h"

void main()
{
    SHM_FD fd,fd1;

    shm_init(1);
    fd= shm_chn_attach();
//    fd1= shm_chn_attach();

    SHARE_BUF_NODE *r_tmp;
    r_tmp= malloc(sizeof(SHARE_BUF_NODE));
    
    char *pull_buf;
    pull_buf= malloc(500);

    int test= 20000;
    while(test--)
    {
        int handle= shm_pull(fd, r_tmp);
        memcpy((void *)pull_buf, (const void *)(r_tmp->share_pt), (r_tmp->share_size));
        printf("sahre mem buffer max size is %d\n",r_tmp->max_buf_size);
        printf("share mem buffer size is %d\n",r_tmp->share_size);
        printf("share mem buffer is %s times=%d\n",pull_buf, test);
        shm_release(handle);
    }

//    shm_pull(fd1, r_tmp);
//    printf("sahre mem buffer max size is %d\n",r_tmp->max_buf_size);
//    printf("share mem buffer size is %d\n",r_tmp->share_size);
//    printf("share mem buffer is %s\n",(unsigned char *)(r_tmp->share_pt));
//    shm_release(fd1);

    getchar();
}
