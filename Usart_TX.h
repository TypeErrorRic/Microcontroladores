/*
 * Usart_TX.h
 *
 * Created: 30/05/2023 6:10:43 p. m.
 *  Author: ricardo
 */ 

#ifndef USART_TX_H_
#define USART_TX_H_

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#define PRESCALER	1024
#define SIZE_SEND_TRAMA		20

//FUNCIONES DE TRASMICIÓN:
void doUSARTinit(unsigned int timer_trasmition);
unsigned char getUSARTudr();
void trasmitirDatos(float (*getVolt) (int *), float (*getAmperios) (int *), short doed);
void setVoltUSART();
void setCorUSART();
void EnviarCadena();


#endif /* USART_TX_H_ */