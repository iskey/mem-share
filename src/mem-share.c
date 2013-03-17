 #include "mem-share.h"



/* add one transfer channel. */
IPC_FD mem_share_channel_add(void)
{

err:
    return -1;
}

IPC_FD mem_share_channel_attach(void)
{
err:
    return -1;
}

int mem_share_push_buffer(IPC_FD mfd, SHARE_BUF_NODE node)
{
err:
    return -1;
}

int mem_share_pull_buffer(IPC_FD mfd, SHARE_BUF_NODE node)
{
err:
    return -1;
}
