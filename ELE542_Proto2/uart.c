#include "uart.h"


/*void sendDebugValue(char* dataToSend, unsigned char valueToSend)
{
	unsigned char stringLenght;

	stringLenght = strlen(dataToSend);
	stringLenght--;
	if (stringLenght >= MAX_STRING_TO_SEND)
	{
		stringLenght = MAX_STRING_TO_SEND;
	}
	uart_send(dataToSend, stringLenght);

	itoa(valueToSend, (char*)string_valueToSend, 10);

	stringLenght = strlen((char*)string_valueToSend);
	if (stringLenght >= MAX_VALUE_TO_SEND)
	{
		stringLenght = MAX_VALUE_TO_SEND;
	}
	uart_send((char*)string_valueToSend, strlen((char*)string_valueToSend));
}
*/

