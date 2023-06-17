/*
 * Usart_RX.c
 *
 * Created: 30/05/2023 6:12:25 p. m.
 *  Author: ricardo
 */ 

void doUSARTinit()
{
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART Receiver: On
	// USART Transmitter: Off
	// USART Mode: Asynchronous
	// USART Baud Rate: 9600
	UCSRA = 0x00;
	UCSRB |= (1<<RXEN);
	UCSRC |= (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
	UBRRH = 0x00;
	UBRRL = 0x19;
}

int isUSARTrxComplete()
{
	if ((UCSRA & (1 << RXC)))
	return 1;
	else
	return 0;
}

int isUSARTerror()
{
	if (((UCSRA & (1 << FE)) | (UCSRA & (1 << DOR)) | (UCSRA & (1 << PE))))
	return 1;
	else
	return 0;
}

unsigned char getUSARTdata()
{
	return UDR;
}
