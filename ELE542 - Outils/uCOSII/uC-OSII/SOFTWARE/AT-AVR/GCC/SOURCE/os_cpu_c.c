/*
**********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                                           AVR Specific code
*
* File         : OS_CPU_C.C
* By           : Ole Saether
* Port Version : V1.01
*
* AVR-GCC port version : 1.0 	2001-04-02 modified/ported to avr-gcc by Jesper Hansen (jesperh@telia.com)
*
*
*
**********************************************************************************************************
*/

#define  OS_CPU_GLOBALS
#include "includes.h"

/*
**********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created. This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              pdata         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack. It is assumed that 'ptos' points to the
*                            highest valid address on the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : Interrupts are enabled when your task starts executing. You can change this by setting the
*              SREG to 0x00 instead. In this case, interrupts would be disabled upon task startup. The
*              application code would be responsible for enabling interrupts at the beginning of the task
*              code. You will need to modify OSTaskIdle() and OSTaskStat() so that they enable
*              interrupts. Failure to do this will make your system crash!
*
*              The AVR return stack is placed 64 bytes above the start of the (512 byte) stack. This
*              provides 32 levels of function call nesting which should be more than enough for most
*              applications (see the "read.me" file for more info).
*
**********************************************************************************************************
*/

void *OSTaskStkInit (void (*task)(void *pd), void *pdata, void *ptos, INT16U opt)
{
    INT8U  *stk;
    INT16U  tmp;

    opt     = opt;                          /* 'opt' is not used, prevent warning                       */
    stk     = (INT8U *)ptos;				/* AVR return stack ("hardware stack")          			*/
    tmp     = (INT16U)task;

	/* "push" initial register values onto the stack */

    *stk-- = (INT8U)tmp;                   /* Put task start address on top of stack          			*/
    *stk-- = (INT8U)(tmp >> 8);

    *stk-- = (INT8U)0x00;                   /* R0  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R1  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R2  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R3  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R4  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R5  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R6  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R7  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R8  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R9  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R10 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R11 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R12 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R13 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R14 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R15 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R16 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R17 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R18 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R19 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R20 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R21 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R22 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R23 = 0x00                                               */
 	
    tmp    = (INT16U)pdata;
    *stk-- = (INT8U)tmp;                    /* Simulate call to function with argument                  */
    *stk-- = (INT8U)(tmp >> 8);				/* R24, R25 contains argument pointer pdata 				*/

    *stk-- = (INT8U)0x00;                   /* R26 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R27 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R28 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R29 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R30 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R31 = 0x00                                               */
    *stk-- = (INT8U)0x80;                   /* SREG = Interrupts enabled                                */
    return ((void *)stk);
}

/*$PAGE*/
#if OS_CPU_HOOKS_EN
/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
void OSTaskSwHook (void)
{
}

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/
void OSTaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
void OSTimeTickHook (void)
{
}
#endif
