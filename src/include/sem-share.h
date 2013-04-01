/******************************************************************************

           This is Iskey.

 ******************************************************************************/
/**
 * @file    mem-share.h
 * @brief   mem-share header files
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

#ifndef __SEM_SHARE__
#define __SEM_SHARE__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef int SEM_FD;

/* init share memory */
extern int sem_init(int work_model);
///* delete share memory */
//extern int sem_uinit();

/* add one share channel. called by sender */
extern SEM_FD sem_trigger_add(int key_id);
/* attach one channel. called by receiver */
extern SEM_FD sem_trigger_attach(int key_id);

/* push block of data to share memory */
extern sem_trigger_lock(SEM_FD sem_fd);
/* pull block of data from share memory */
extern sem_trigger_unlock(SEM_FD sem_fd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __SEM_SHARE__ */
