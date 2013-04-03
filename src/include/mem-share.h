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

#ifndef __MEM_SHARE__
#define __MEM_SHARE__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define PUSH_MODEL 0
#define PULL_MODEL 1

typedef int SHM_FD;

/* share memory type */
typedef struct __SHARE_BUF_NODE__{
    int max_buf_size;//the share buffer size
    int share_size;//the share block size in the share buffer
    void *share_pt;//the share address
}SHARE_BUF_NODE;

/* init share memory */
extern int shm_init(int work_model);
/* delete share memory */
extern int shm_uinit();

/* add one share channel. called by sender */
extern SHM_FD shm_chn_add(int max_buf_size);
/* attach one channel. called by receiver */
extern SHM_FD shm_chn_attach(void);

/* push block of data to share memory */
extern shm_push(SHM_FD mfd, SHARE_BUF_NODE *node);
/* pull block of data from share memory */
extern shm_pull(SHM_FD mfd, SHARE_BUF_NODE *node);;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __MEM_SHARE__ */
