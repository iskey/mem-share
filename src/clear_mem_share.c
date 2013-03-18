#include <stdio.h>

#include "mem-share.h"

void main()
{
    shm_init(0);
    shm_uinit();
    printf("share mem uinit successfully");
}
