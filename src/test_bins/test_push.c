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
#include <fcntl.h>

#include "mem-share.h"

void main(int argc, char* argv[])
{
    SHM_FD fd;
    
    if(argc< 2){//check args
        printf("usage: test_push filename!\n");
    }
    
    char rd_buf[5000];//input buffer
    int file_fd;//input file path
    ssize_t share_size;//read size
    
    file_fd= open(argv[1], O_RDONLY);//open file
    if(-1== file_fd){
        printf("file open error!\n");
    }
    
    SHARE_BUF_NODE *tmp;
    tmp= malloc(sizeof(SHARE_BUF_NODE));
    
    /* Initial shm modle */
    shm_init(0);
    /* add shm channel with max buffer size */
    fd= shm_chn_add(800000);
    if(-1== fd){
        printf("shm channel add error!\n");
        return;
    }

    while(1)
    {
        /* read file from input file */
        share_size= read(file_fd, rd_buf, 5000);
        if(0>= share_size){
            if(-1== share_size){
                printf("read file error!\n");              
            }
            else if(0== share_size){
                printf("push successfully!\n");
            }
            return;
        }

        tmp->share_pt= rd_buf;
        tmp->share_size= share_size;
        shm_push(fd, tmp);
        printf("share channel %d is pushed successfully by share size= %d\n", fd, tmp->share_size);
    }

    getchar();
}
