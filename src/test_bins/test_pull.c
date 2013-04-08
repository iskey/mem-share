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

void main(int argc, char* argv[])
{
    SHM_FD fd,fd1;

    shm_init(1);
    fd= shm_chn_attach();

    SHARE_BUF_NODE *r_tmp;
    r_tmp= malloc(sizeof(SHARE_BUF_NODE));
    
    if(argc< 2){
        printf("usage: test_pull filename!\n");
    }
    
    FILE* file_fd;//output file path
    ssize_t wr_size;//write size 
    
    file_fd= fopen(argv[1], "wb");//open output file
    if(NULL== file_fd){
        printf("file open error!\n");
        return;
    }
    
    while(1)
    {
        int handle= shm_pull(fd, r_tmp);//pull data from shm channel

        wr_size= fwrite(r_tmp->share_pt, 1, r_tmp->share_size, file_fd);//write data to output file
        if(-1== wr_size){
            printf("write error!\n");
            return ;
        }
        printf("share mem buffer size is %d\n",r_tmp->share_size);
        shm_release(handle);
        
        if(5000!= wr_size){
            fclose(file_fd);
            return;
        }
    }
}
