/*
 * Usart_RX.h
 *
 * Created: 30/05/2023 6:12:07 p. m.
 *  Author: ricardo
 */ 


#ifndef USART_RX_H_
#define USART_RX_H_

void doConfigureIO();
void doUSARTinit();
int isUSARTrxComplete();
int isUSARTerror();
unsigned char getUSARTdata();

#endif /* USART_RX_H_ */