 #include <unistd.h>
 #include <stdlib.h>
#include <stdio.h>

#include <sys/sem.h>

static int set_semvalue(void);
static void del_semvalue(void);
static int sem_p(void);
static int sem_v(void);


