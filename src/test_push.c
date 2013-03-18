#include <stdio.h>

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
    printf("size is %d\n",tmp->share_size);
    shm_push(fd, tmp);

    tmp->share_size= sizeof("yyyyyyyyyyyyycccccccccc");
    tmp->share_pt= "yyyyyyyyyyyyycccccccccc";
    shm_push(fd1, tmp);
}
