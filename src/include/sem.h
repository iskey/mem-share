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

extern int set_semvalue(void);
extern void del_semvalue(void);
extern int sem_p(void);
extern int sem_v(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __SEM_COMMON__ */
