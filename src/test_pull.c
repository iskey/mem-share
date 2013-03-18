#include <stdio.h>

#include "mem-share.h"

void main()
{
    IPC_FD fd,fd1;

    shm_init(1);
    fd= shm_chn_attach();
    fd1= shm_chn_attach();

    SHARE_BUF_NODE *r_tmp;
    r_tmp= malloc(sizeof(SHARE_BUF_NODE));

    shm_pull(fd, r_tmp);
    printf("share mem buffer size is %d\n",r_tmp->share_size);
    printf("share mem buffer is %s\n",(unsigned char *)(r_tmp->share_pt));

    shm_pull(fd1, r_tmp);
    printf("share mem buffer size is %d\n",r_tmp->share_size);
    printf("share mem buffer is %s\n",(unsigned char *)(r_tmp->share_pt));
}
