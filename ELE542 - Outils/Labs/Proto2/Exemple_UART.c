#include <includes.h>
#include <stdio.h>

#define Pi (3.1415926535897932384626433832795)

float AngleD	= 0.0;
float VitesseD	= 0.0;

INT8U tpRx = 0;

#define STANDARD_STACK_SIZE 100
#define DEBUG_STACK_SIZE 150

OS_STK CmdTele_Stk[STANDARD_STACK_SIZE];
OS_STK SendDbg_Stk[STANDARD_STACK_SIZE];
OS_STK MaTache_Stk[DEBUG_STACK_SIZE];

OS_EVENT	*UARTRxBox;
OS_EVENT	*UARTTxSem;
OS_EVENT	*UARTEchoSem;
OS_EVENT	*DebugMessageQ;

void *DebugMessage[10];

extern void InitOSTimer(void);

int  main(void);
       
static  void  CmdTeleTask(void *p_arg);
static  void  SendDbgTask(void *p_arg);
static  void  MaTacheTask(void *p_arg);

SIGNAL (SIG_UART_RECV) {
	OS_INT_ENTER();
	tpRx = UDR;
	OSMboxPost(UARTRxBox, &tpRx);
	OS_INT_EXIT();
}

SIGNAL (SIG_UART_TRANS) {
	OS_INT_ENTER();
	OSSemPost(UARTTxSem);
	OS_INT_EXIT();
}


void InitUART(void) {
	UCSRB = 0x00;
	UBRRH = 0;
	UBRRL = 103;	//9600Kbps@16Mhz (ATMega32)
	UCSRA = 0x00;
	UCSRC = 0x86;
	UCSRB = 0xD8;
}

int main(void) {
	cli();
	OSInit();
	InitUART();
      OSTaskCreate(CmdTeleTask, NULL, (OS_STK *)&CmdTele_Stk[STANDARD_STACK_SIZE-1], 4); 
      OSTaskCreate(SendDbgTask, NULL, (OS_STK *)&SendDbg_Stk[STANDARD_STACK_SIZE-1], 5); 
      OSTaskCreate(MaTacheTask, NULL, (OS_STK *)&MaTache_Stk[DEBUG_STACK_SIZE-1], 6); 
	UARTTxSem = OSSemCreate(1);
	UARTEchoSem = OSSemCreate(1);
	DebugMessageQ = OSQCreate(&DebugMessage[0],10);
	OSStart();
}


static  void  CmdTeleTask(void *p_arg) {
	INT8U	err;
	short	tp;
	INT8U	Etat = 0;

    (void)p_arg;          // Prevent compiler warnings

    cli();
	InitOSTimer();
	UARTRxBox = OSMboxCreate(NULL);
    sei();

    while (TRUE) {               // Task body, always written as an infinite loop.
        tp = *((short *) OSMboxPend(UARTRxBox, 0, &err));
		if (OSSemAccept(UARTEchoSem)) {
			OSSemPend(UARTTxSem, 0, &err);
			UDR = tp;
			OSSemPost(UARTEchoSem);
		}
		switch(Etat) {
		case 0 :	if (tp == 0xF0) {
						VitesseD = 0.0;
					} else if (tp == 0xF1) {
						Etat = 1;
					}
					break;
		case 1 :	VitesseD = ((float) tp) - 100.0;
					Etat = 2;
					break;
		case 2 :	AngleD = ((float) tp)*Pi/90.0;
					break;
		}
    }
}


static  void  SendDbgTask(void *p_arg) {
	INT8U	err;
	char	*p;
	
    (void)p_arg;          // Prevent compiler warnings

    while (TRUE) {               // Task body, always written as an infinite loop.
        p = (char *) OSQPend(DebugMessageQ, 0, &err);
		OSSemPend(UARTEchoSem, 0, &err);
		OSSemPend(UARTTxSem, 0, &err);
		UDR = 0xFE;
		while (*p != 0) {
			OSSemPend(UARTTxSem, 0, &err);
			UDR = *p++;
		}
		OSSemPend(UARTTxSem, 0, &err);
		UDR = 0xFF;
		OSSemPost(UARTEchoSem);
    }
}


static  void  MaTacheTask(void *p_arg) {
	INT8U	err;
	char	message[20];
	INT8U	i = 0;
	
    (void)p_arg;          // Prevent compiler warnings

    while (TRUE) {               // Task body, always written as an infinite loop.
		sprintf(message, "mon message %u",i++);
		OSQPost(DebugMessageQ, (void *) message);
		OSTimeDly(1000);
    }
}


