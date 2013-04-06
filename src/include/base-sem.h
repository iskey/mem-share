/******************************************************************************

           This is Iskey.

 ******************************************************************************/
/**
 * @file    sem.h
 * @brief   sem header files.
 * @author  iskey@outlook.com
 * @version Initial Draft
 * @note    none
 * @date    2013/3/19
 */
/******************************************************************************
 *  Function List :
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

#ifndef __SEM_COMMON__
#define __SEM_COMMON__

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/* create semaphore */
extern int create_sem(int key_id, int count);
/* attach semaphore */
extern int attach_sem(int key_id, int count);
/* delete semaphore */
extern int free_sem(int semid);
/* set semaphore value */
extern int set_sem_value(int semid, int index, int value);
/* get semaphore value */
extern int get_sem_value(int semid, int index);
/* block when semid[index] is negative and ms has not timeout. */
extern int try_lock_sem(int semid, int index, int ms);
/* block when sem[index] is negative, until it become plus */
extern int lock_sem(int semid, int index);
/* unlock semaphore */
extern int unlock_sem(int semid, int index);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __SEM_COMMON__ */
