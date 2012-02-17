/************************************************************************************
*                            APPLICATION SPECIFIC SAMPLE CODE
*
*
* File : TEST.C
* By   : Bruno De Kelper
************************************************************************************/

#include <includes.h>

extern void InitOSTimer(void);
/***********************************************************************************
*                                     VARIABLES
***********************************************************************************/
#define OS_TASK_STK_SIZE      100

OS_STK  Task1_Stk[OS_TASK_STK_SIZE];
OS_STK  Task2_Stk[OS_TASK_STK_SIZE];
OS_STK  Task3_Stk[OS_TASK_STK_SIZE];
OS_STK  Task4_Stk[OS_TASK_STK_SIZE];
OS_STK  Task5_Stk[OS_TASK_STK_SIZE];
OS_STK  Task6_Stk[OS_TASK_STK_SIZE];
OS_STK  Task7_Stk[OS_TASK_STK_SIZE];
OS_STK  Task8_Stk[OS_TASK_STK_SIZE];

/***********************************************************************************
*                                           FUNCTION PROTOTYPES
***********************************************************************************/
char tpRx = 0;

int  main(void);
       
static  void  Task1(void *p_arg);
static  void  Task2(void *p_arg);
static  void  Task3(void *p_arg);
static  void  Task4(void *p_arg);
static  void  Task5(void *p_arg);
static  void  Task6(void *p_arg);
static  void  Task7(void *p_arg);
static  void  Task8(void *p_arg);

void  LED_Init (void);
void  LED_On (INT8U led);
void  LED_Off (INT8U led);
void  LED_Toggle (INT8U led);

volatile long unsigned int i;

SIGNAL (SIG_OUTPUT_COMPARE0) {
	OS_INT_ENTER();
//
//	Placer le code ici
//
	OS_INT_EXIT();
}

SIGNAL (SIG_UART_RECV) {
cli();
//	OS_INT_ENTER();
//PORTB &= ~0x02;
for (i = 0; i < 0xFFFFFFFF; i++);
PORTB = 0x00;
	tpRx = UDR;
		while (!(UCSRA & 0x20));
	UDR = tpRx;
//	OSMboxPost(UARTRxBox, &tpRx);
//PORTB &= ~0x04;
//	OS_INT_EXIT();
sei();
}

/************************************************************************************
*                                    MAIN
************************************************************************************/

int  main (void)
{
    OSInit();

    OSTaskCreate(Task1, NULL, (OS_STK *)&Task1_Stk[OS_TASK_STK_SIZE-1], 4); 
    OSTaskCreate(Task2, NULL, (OS_STK *)&Task2_Stk[OS_TASK_STK_SIZE-1], 5); 
    OSTaskCreate(Task3, NULL, (OS_STK *)&Task3_Stk[OS_TASK_STK_SIZE-1], 6); 
    OSTaskCreate(Task4, NULL, (OS_STK *)&Task4_Stk[OS_TASK_STK_SIZE-1], 7); 
    OSTaskCreate(Task5, NULL, (OS_STK *)&Task5_Stk[OS_TASK_STK_SIZE-1], 8); 
    OSTaskCreate(Task6, NULL, (OS_STK *)&Task6_Stk[OS_TASK_STK_SIZE-1], 9); 
    OSTaskCreate(Task7, NULL, (OS_STK *)&Task7_Stk[OS_TASK_STK_SIZE-1], 10); 
    OSTaskCreate(Task8, NULL, (OS_STK *)&Task8_Stk[OS_TASK_STK_SIZE-1], 11); 

	UCSRB = 0x00;
	UBRRH = 0;
	UBRRL = 103;	// 9.6 Kbps @ 16Mhz (ATMega32)
	UCSRA = 0x00;
	UCSRC = 0x86;
	UCSRB = 0x98;


    OSStart();
}

/*************************************************************************************
*                                 STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*************************************************************************************/

static  void  Task1 (void *p_arg)
{
    (void)p_arg;          // Prevent compiler warnings

//    cli();

	InitOSTimer();

    LED_Init();

    DDRD  = 0xFF;
    PORTD = 0x00;

    DDRB  = 0xFF;
    PORTB = 0x00;

	// Initialise Timer0 simplement pour avoir une deuxième intérruption
//    TCCR0  = 0x0B;
//    TCNT0  =    0;         // Start TCNT at 0 for a new cycle
//    OCR0   =  124;

//    TIFR   |= 0x03;        // Clear  TIMER2 compare Interrupt Flag
//    TIMSK  |= 0x02;        // Enable TIMER2 compare Interrupt

//    sei();

    while (TRUE) {               // Task body, always written as an infinite loop.
        LED_Toggle(1);
        OSTimeDly(200);
    }
}


/**************************************************************************
*                               TASK #2
**************************************************************************/

static  void  Task2 (void *p_arg)
{
 	INT8U   err;

   (void)p_arg;
 
    while (TRUE) {    // Task body, always written as an infinite loop.
        LED_Toggle(2);
        OSTimeDly(400);
    }
}

/**************************************************************************
*                               TASK #3
**************************************************************************/

static  void  Task3 (void *p_arg)
{
    (void)p_arg;

    while (TRUE) {                          /* Task body, always written as an infinite loop.          */
        LED_Toggle(3);
        OSTimeDly(600);
    }
}


/**************************************************************************
*                               TASK #4
**************************************************************************/

static  void  Task4 (void *p_arg)
{
    (void)p_arg;

    while (TRUE) {     // Task body, always written as an infinite loop.
        LED_Toggle(4);
        OSTimeDly(800);
    }
}


/**************************************************************************
*                               TASK #5
**************************************************************************/

static  void  Task5 (void *p_arg)
{
    (void)p_arg;

    while (TRUE) {    // Task body, always written as an infinite loop.
        LED_Toggle(5);
        OSTimeDly(1000);
    }
}

/**************************************************************************
*                               TASK #6
**************************************************************************/

static  void  Task6 (void *p_arg)
{
    (void)p_arg;

    while (TRUE) {    // Task body, always written as an infinite loop.
        LED_Toggle(6);
        OSTimeDly(1200);
    }
}


/**************************************************************************
*                               TASK #7
**************************************************************************/

static  void  Task7 (void *p_arg)
{
    (void)p_arg;

    while (TRUE) {    // Task body, always written as an infinite loop.
        LED_Toggle(7);
        OSTimeDly(1400);
    }
}


/**************************************************************************
*                               TASK #8
**************************************************************************/

static  void  Task8 (void *p_arg)
{
    (void)p_arg;

    while (TRUE) {    // Task body, always written as an infinite loop.
        LED_Toggle(8);
        OSTimeDly(1600);
    }
}



/********************************************************************
*********************************************************************/

void  LED_Init (void)
{
    DDRB  = 0xFF;    // All PORTB pins are outputs
    PORTB = 0xFF;
    LED_Off(0);     // Turn ON all the LEDs
}


/********************************************************************
*                             LED ON
*
* Description : This function is used to control any or all the LEDs on the board.
*               
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want ALL the LEDs to be ON
*                      1    turns ON LED1 on the board
*                      .
*                      .
*                      8    turns ON LED8 on the board
*********************************************************************/

void  LED_On (INT8U led)
{
    switch (led) {
        case 0:
             PORTB  =  0x00;
             break;

        case 1:
             PORTB &= ~0x01;
             break;

        case 2:
             PORTB &= ~0x02;
             break;

        case 3:
             PORTB &= ~0x04;
             break;

        case 4:
             PORTB &= ~0x08;
             break;

        case 5:
             PORTB &= ~0x10;
             break;

        case 6:
             PORTB &= ~0x20;
             break;

        case 7:
             PORTB &= ~0x40;
             break;

        case 8:
             PORTB &= ~0x80;
             break;
    }
}

/**************************************************************************
*                             LED OFF
*
* Description : This function is used to control any or all the LEDs on the board.
*               
* Arguments   : led    is the number of the LED to turn OFF
*                      0    indicates that you want ALL the LEDs to be OFF
*                      1    turns OFF LED1 on the board
*                      .
*                      .
*                      8    turns OFF LED8 on the board
****************************************************************************/

void  LED_Off (INT8U led)
{
    switch (led) {
        case 0:
             PORTB  =  0xFF;
             break;

        case 1:
             PORTB |=  0x01;
             break;

        case 2:
             PORTB |=  0x02;
             break;

        case 3:
             PORTB |=  0x04;
             break;

        case 4:
             PORTB |=  0x08;
             break;

        case 5:
             PORTB |=  0x10;
             break;

        case 6:
             PORTB |=  0x20;
             break;

        case 7:
             PORTB |=  0x40;
             break;

        case 8:
             PORTB |=  0x80;
             break;
    }
}

/*******************************************************************************
*                         LED TOGGLE
*
* Description : This function is used to toggle the state of any or all the LEDs on the board.
*               
* Arguments   : led    is the number of the LED to toggle
*                      0    indicates that you want ALL the LEDs to toggle
*                      1    Toggle LED1 on the board
*                      .
*                      .
*                      8    Toggle LED8 on the board
******************************************************************************/

void  LED_Toggle (INT8U led)
{
    switch (led) {
        case 0:
             PORTB ^=  0xFF;
             break;

        case 1:
             PORTB ^=  0x01;
             break;

        case 2:
             PORTB ^=  0x02;
             break;

        case 3:
             PORTB ^=  0x04;
             break;

        case 4:
             PORTB ^=  0x08;
             break;

        case 5:
             PORTB ^=  0x10;
             break;

        case 6:
             PORTB ^=  0x20;
             break;

        case 7:
             PORTB ^=  0x40;
             break;

        case 8:
             PORTB ^=  0x80;
             break;
    }
}
