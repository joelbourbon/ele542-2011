/*
**************************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*
*                                             AVR Sample code
* File        : TEST.C
* By          : Ole Saether
* Version     : V1.01
*
* AVR-GCC port version : 1.0 	2001-04-02 modified/ported to avr-gcc by Jesper Hansen (jesperh@telia.com)
*
*
* Description :
*
*         This file contains a simple example program showing how to use the AVR port of uC/OS-II. It is
*         based on Example #1 from Jean Labrosse's book "MicroC/OS-II, The Real Time Kernel." The main
*         difference is that this example does not display the time of day and the uC/OS-II version number.
*         You must have the AVR UART connected to a VT102 compatible terminal (HyperTerminal in Windows is OK)
*         to get the most out of this example.
*
*         The support routines at the end of this file are included only to make this example run; they should
*         not be used in production code without careful testing.
**************************************************************************************************************
*/

#include "includes.h"

/*
**************************************************************************************************************
*                                               CONSTANTS
**************************************************************************************************************
*/

#define  TASK_STK_SIZE  OS_TASK_DEF_STK_SIZE            /* Size of each task's stacks (# of bytes)          */
#define  N_TASKS        10                              /* Number of identical tasks                        */

/*
**************************************************************************************************************
*                                               VARIABLES
**************************************************************************************************************
*/

OS_STK          TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                     */
OS_STK          TaskStartStk[TASK_STK_SIZE];
char            TaskData[N_TASKS];                      /* Parameters to pass to each task                  */
OS_EVENT       *RandomSem;
OS_EVENT       *DispStrSem;
INT32U          RndNext;                                /* Used by random generator                         */
INT8U           UartTxBuf[256];                         /* 256 bytes UART transmit buffer                   */
INT8U           UartTxRdPtr;                            /* UART transmit buffer read pointer                */
INT8U           UartTxWrPtr;                            /* UART transmit buffer write pointer               */
INT8U           UartTxCount;                            /* Number of characters to send                     */
OS_EVENT       *UartTxSem;


unsigned char kurt;

/*
**************************************************************************************************************
*                                           FUNCTION PROTOTYPES
**************************************************************************************************************
*/

void  Task(void *data);                                 /* Function prototypes of tasks                     */
void  TaskStart(void *data);                            /* Function prototypes of Startup task              */
void  PutChar(char c);                                  /* Write a character to the AVR UART                */
void  AvrInit(void);                                    /* Initialize AVR                                   */
void  PutString(const char *s);                         /* Write a null-terminated string to the AVR UART   */
void  SPrintDec(char *, INT16U, INT8U);                 /* Output an INT16U to a string (right adjust)      */
void  PutDec (INT8U x);                                 /* Display an INT8U without leading zeros           */
void  VT102Attribute (INT8U fgcolor, INT8U bgcolor);    /* Set attributes on VT102 terminal                 */
void  VT102DispClrScr(void);                            /* Clear VT102 terminal                             */
void  VT102DispChar(INT8U, INT8U, char, INT8U, INT8U);  /* Display a character on VT102 terminal            */
void  VT102DispStr(INT8U, INT8U, char *, INT8U, INT8U); /* Display a string on VT102 terminal               */
INT8U random(INT8U n);                                  /* Simple random generator (found in K&R)           */

/*
**************************************************************************************************************
*                                                MAIN
**************************************************************************************************************
*/

int main (void)
{
    AvrInit();                                          /* Initialize the AVR UART and Timer                */
    OSInit();
    RandomSem  = OSSemCreate(1);                        /* Random number semaphore                          */
    DispStrSem = OSSemCreate(1);                        /* Display string semaphore                         */
    UartTxSem  = OSSemCreate(254);                      /* Initialize Uart transmit buffer semaphore to 2   */

    OSTaskCreate(TaskStart, (void *)0, (void *)&TaskStartStk[TASK_STK_SIZE - 1], 0);

    RndNext = 1;                                        /* set random generator seed to 1                   */
    OSStart();                                          /* Start multitasking                               */
    return 0;
}

/*
**************************************************************************************************************
*                                              STARTUP TASK
**************************************************************************************************************
*/
void TaskStart (void *data)
{
    INT8U   i;
    INT8U   err;
    char    s[10];

    data = data;                                        /* Prevent compiler warning                         */

    VT102DispClrScr();                                  /* Clear the screen                                 */
    VT102DispStr(26,  1, "uC/OS-II, The Real-Time Kernel", COLOR_WHITE, COLOR_RED);
    VT102DispStr(33,  2, "Jean J. Labrosse", COLOR_WHITE, COLOR_BLACK);
    VT102DispStr(29,  3, "AVR port by Ole Saether", COLOR_WHITE, COLOR_BLACK);
    VT102DispStr(29,  4, "gcc port by Jesper Hansen", COLOR_WHITE, COLOR_BLACK);
    VT102DispStr(1,  23, "Determining  CPU's capacity ...", COLOR_WHITE, COLOR_BLACK);

    OSStatInit();                                       /* Initialize uC/OS-II's statistics                 */
    
    for (i = 0; i < N_TASKS; i++) {                     /* Create N_TASKS identical tasks                   */
        TaskData[i] = '0' + i;                          /* Each task will display its own letter            */
        OSTaskCreate(Task, (void *)&TaskData[i], (void *)&TaskStk[i][TASK_STK_SIZE - 1], i + 1);
    }
    VT102DispStr(1, 23, "#Tasks          : xxxxx  CPU Usage: xxx %", COLOR_WHITE, COLOR_BLACK);
    VT102DispStr(1, 24, "#Task switch/sec: xxxxx", COLOR_WHITE, COLOR_BLACK);
    for (;;) {
        SPrintDec(s, (INT16U)OSTaskCtr, 5);             /* Display #tasks running                           */
        VT102DispStr(19, 23, s, COLOR_WHITE, COLOR_BLUE);
        SPrintDec(s, (INT16U)OSCPUUsage, 3);            /* Display CPU usage in %                           */
        VT102DispStr(37, 23, s, COLOR_WHITE, COLOR_BLUE);
        SPrintDec(s, (INT16U)OSCtxSwCtr, 5);            /* Display #context switches per second             */
        VT102DispStr(19, 24, s, COLOR_WHITE, COLOR_BLUE);
        OSCtxSwCtr = 0;
        OSTimeDlyHMSM(0, 0, 1, 0);                      /* Wait one second                                  */
    }
}

/*
**************************************************************************************************************
*                                                  TASKS
**************************************************************************************************************
*/
void Task (void *data)
{
    INT8U x;
    INT8U y;
    INT8U err;

    for (;;) {
        OSSemPend(RandomSem, 0, &err);                  /* Acquire semaphore to perform random numbers      */
        x = random(80);                                 /* Find X position where task number will appear    */
        y = random(15);                                 /* Find Y position where task number will appear    */
        OSSemPost(RandomSem);                           /* Release semaphore                                */
                                                        /* Display the task number on the screen            */
        VT102DispChar(x + 1, y + 6, *(char *)data, COLOR_WHITE, COLOR_BLACK);
        OSTimeDly(3);                                   /* Delay 3 clock ticks                              */
    }
}

/*
**************************************************************************************************************
*                                                  SUPPORT ROUTINES
**************************************************************************************************************
*/

SIGNAL(SIG_UART_TRANS)      
{
    UartTxCount--;                                      /* Decrement number of characters left to send      */
    if (UartTxCount) {
        outp(UartTxBuf[UartTxRdPtr],UDR);               /* Place next character into UART transmit register */
        UartTxRdPtr++;                                  /* Advance to next character                        */
        OSSemPost(UartTxSem);                           /* Signal that we have room for one more character  */
    } else {
        outp(inp(UCR) & ~0x40,UCR);                     /* Disable UART transmit complete interrupt         */
    }
}

void AvrInit (void)
{
    outp(0x05,	TCCR0);                                 /* Set TIMER0 prescaler to CLK/1024                 */
    outp(0x02,  TIMSK);                                 /* Enable TIMER0 overflow interrupt                 */

    outp(35,	UBRR);                                  /* 19200 BAUD at 11.059200MHz                          */

    outp((1<<SRE)/*|(1<<SRW)*/, MCUCR);                   /* Enable external RAM                              */
    outp(0x08,	UCR);                                   /* Enable UART transmitter                          */
    outp(inp(UDR) | 0x40,UDR);                         /* Clear UART transmit complete bit (TXC)           */

    UartTxCount = 0;                                    /* Clear number of characters to send               */
    UartTxRdPtr = 0;                                    /* Initialize transmit buffer read pointer          */
    UartTxWrPtr = 0;                                    /* Initialize transmit buffer write pointer         */
}


void PutChar (char c)
{
    INT8U err;

    if (UartTxCount) {
        OSSemPend(UartTxSem, 0, &err);                  /* Wait for space in transmit buffer                */
    }
    OS_ENTER_CRITICAL();
    if (UartTxCount) {
        UartTxBuf[UartTxWrPtr] = c;                     /* Put character to send in transmit buffer         */
        UartTxWrPtr++;                                  /* Prepare for next character                       */
        UartTxCount++;                                  /* Increment number of characters to send           */
    } else {
        outp(c,	UDR);                                /* Write first char directly to the UART            */
        UartTxCount = 1;
        outp(inp(UCR) | 0x40,UCR);                             /* Enable UART transmit complete interrupt          */
    }
    OS_EXIT_CRITICAL();
}


void PutString (const char *s)
{
    while (*s != '\0') {
        PutChar(*s++);
    }
}


void SPrintDec(char *s, INT16U x, INT8U n)
{
    INT8U i;


    s[n] = '\0';
    for (i = 0; i < n; i++) {
        s[n - i - 1] = '0' + (x % 10);
        x /= 10;
    }
    for (i = 0; i < (n - 1); i++) {
        if (s[i] == '0') {
            s[i] = ' ';
        } else {
            break;
        }
    }
}


INT8U random (INT8U n)
{
    RndNext = RndNext * 1103515245 + 12345;
    return ((INT8U)(RndNext / 256) % (n + 1));
}


void PutDec (INT8U x2)
{
    INT8U x0;
    INT8U x1;


    x0  = (x2 % 10);
    x2 /= 10;
    x1  = (x2 % 10);
    x2 /= 10;
    if (x2) {
        PutChar(x2 + '0');
    }
    if (x1 || x2) {
        PutChar(x1 + '0');
    }
    PutChar(x0 + '0');
}


void VT102Attribute (INT8U fgcolor, INT8U bgcolor)
{
    PutChar(0x1b);
    PutChar('[');
    PutDec(30 + fgcolor);
    PutChar(';');
    PutDec(40 + bgcolor);
    PutChar('m');
}


void VT102DispClrScr (void)
{
    VT102Attribute(COLOR_WHITE, COLOR_BLACK);
    PutString("\x1B[2J");
}


void VT102DispChar (INT8U x, INT8U y, char c, INT8U fgcolor, INT8U bgcolor)
{
    INT8U err;

    OSSemPend(DispStrSem, 0, &err);                     /* Acquire semaphore to display string              */
    VT102Attribute(fgcolor, bgcolor);
    PutChar(0x1B);
    PutChar('[');
    PutDec(y);
    PutChar(';');
    PutDec(x);
    PutChar('H');
    PutChar(c);
    OSSemPost(DispStrSem);                              /* Release semaphore                                */
}


void VT102DispStr (INT8U x, INT8U y, char *s, INT8U fgcolor, INT8U bgcolor)
{
    INT8U err;

    OSSemPend(DispStrSem, 0, &err);                     /* Acquire semaphore to display string              */
    VT102Attribute(fgcolor, bgcolor);
    PutChar(0x1B);
    PutChar('[');
    PutDec(y);
    PutChar(';');
    PutDec(x);
    PutChar('H');
    PutString(s);
    OSSemPost(DispStrSem);                              /* Release semaphore                                */
}
