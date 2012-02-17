/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                                         Configuration for AVR
*
* File    : OS_CFG.H
* By      : Ole Saether
* Version : V1.01
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         uC/OS-II CONFIGURATION
*********************************************************************************************************
*/
#define OS_TASK_DEF_STK_SIZE    512    /* Default stack size                                           */

#define OS_MAX_EVENTS             3    /* Max. number of event control blocks in your application ...  */
                                       /* ... MUST be >= 2                                             */
#define OS_MAX_MEM_PART           2    /* Max. number of memory partitions ...                         */
                                       /* ... MUST be >= 2                                             */
#define OS_MAX_QS                 2    /* Max. number of queue control blocks in your application ...  */
                                       /* ... MUST be >= 2                                             */
#define OS_MAX_TASKS             11    /* Max. number of tasks in your application ...                 */
                                       /* ... MUST be >= 2                                             */

#define OS_LOWEST_PRIO           12    /* Defines the lowest priority that can be assigned ...         */
                                       /* ... MUST NEVER be higher than 63!                            */

#define OS_TASK_IDLE_STK_SIZE   OS_TASK_DEF_STK_SIZE /* Idle task stack size (# of 16-bit wide entries)*/

#define OS_TASK_STAT_EN           1    /* Enable (1) or Disable(0) the statistics task                 */
#define OS_TASK_STAT_STK_SIZE   OS_TASK_DEF_STK_SIZE /* Statistics task stack size (# of 16-bit wide entries) */

#define OS_CPU_HOOKS_EN           1    /* uC/OS-II hooks are found in the processor port files         */
#define OS_MBOX_EN                0    /* Exclude code for MAILBOXES                                   */
#define OS_MEM_EN                 0    /* Exclude code for MEMORY MANAGER (fixed sized memory blocks)  */
#define OS_Q_EN                   0    /* Exclude code for QUEUES                                      */
#define OS_SEM_EN                 1    /* Include code for SEMAPHORES                                  */
#define OS_TASK_CHANGE_PRIO_EN    0    /* Exclude code for OSTaskChangePrio()                          */
#define OS_TASK_CREATE_EN         1    /* Include code for OSTaskCreate()                              */
#define OS_TASK_CREATE_EXT_EN     0    /* Exclude code for OSTaskCreateExt()                           */
#define OS_TASK_DEL_EN            0    /* Exclude code for OSTaskDel()                                 */
#define OS_TASK_SUSPEND_EN        0    /* Exclude code for OSTaskSuspend() and OSTaskResume()          */

#define OS_TICKS_PER_SEC         50    /* Set the number of ticks in one second (7.3728 MHz AVR clock) */
