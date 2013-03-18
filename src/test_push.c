#include <stdio.h>

#include "mem-share.h"

void main()
{
    IPC_FD fd,fd1;

    shm_init(0);
    fd= shm_chn_add(1024);
    fd1= shm_chn_add(2048);

    SHARE_BUF_NODE *tmp;
    tmp= malloc(sizeof(SHARE_BUF_NODE));

    tmp->share_size= sizeof("WWWXXXJKOJKOO");
    tmp->share_pt= "WWWXXXJKOJKOO";
    shm_push(fd, tmp);

    tmp->share_size= sizeof("yyyyyyyyyyyyycccccccccc");
    tmp->share_pt= "yyyyyyyyyyyyycccccccccc";
    shm_push(fd1, tmp);
}
