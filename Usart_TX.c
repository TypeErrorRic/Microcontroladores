/*
 * Usart.c
 *
 * Created: 30/05/2023 6:10:20 p. m.
 *  Author: ricardo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Usart_TX.h"

#ifndef F_CPU
#define F_CPU	16000000UL
#endif

//VARAIABLE DE TRASMICIÓN:
static short initTrasmit = 0;
static short stop = 0;

//ESTRUCTURA DE ENVIO:
static unsigned char buffer[SIZE_SEND_TRAMA];
static unsigned int pointer = 0;

//CONFIGURAR EN MILISGUNDOS:
void doUSARTinit(unsigned int timer_trasmition)
{
	UCSR0A = 0X00;
	UCSR0A |= (1 << UDRE0); //HABILITANDO UCSR0C.
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); //RECEPCIÓN ACTIVADA CON INTERRUPCIONES. TRASMICIÓN ON.
	UCSR0C |= (1 << UCSZ01) | (1<<UCSZ00); //TRASMICIÓN DE 8 BITS.
	UCSR0C &= ~(1 << UMSEL01) & ~(1 << UMSEL00); //NO PARIDAD.
	//VELOCIDAD DE TRASMICIÓN DE BITS: 9600 BAUDIOS A 16MHZ.
	UBRR0H = 0x00; 
	UBRR0L = 0x67;
	
	//CONFIGURACIÓN DE EVENTO DE DISPARO: //USAREMOS TIMER.
	TCCR1A = 0x00;
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); //PRESCALER 1024.
	TIMSK1 |= (1 << OCIE1A); //ACTIVANDO MODO DE INTERRUPCIONES:
	unsigned int aux = (timer_trasmition*(F_CPU/PRESCALER))/1000;
	OCR1A = aux > 65530 ? 65530 : aux;
	
	sei(); //ACTIVAR INTERRUPCIONES.
	
	//INICIALIZAR LA TRASMICIÓN:
	UDR0 = 0x00;
	
	//INICIALIZAR BUFFER:
	for(short i=0; i < SIZE_SEND_TRAMA; i++) buffer[i] = '\0';
}

ISR(USART_RX_vect)
{
	switch(UDR0)
	{
	case '1':
		//INICIAR TRASMICIÓN:
		initTrasmit = 1;
		PORTD |= (1 << 4);
		stop = 0;
		break;
	case '2':
		stop = 1;
		//PORTD &= ~(1 << 4);
	default:
		break;	
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(initTrasmit)
	{
		trasmitirDatos(NULL,NULL, 1);
		//RESTABLECER CONTADOR:
		TCNT1 = 0;	
	}
}

////////////////////////////FUNCIÓN PARA ENVIÓ DE DATOS/////////////////////////////////

//FUNCIÓN DE ALMACENAMIENTO DE DATOS:
static void asignar(const char *valor)
{
	short i = 0;
	while(1)
	{
		if ((*(valor + i) != '\0') && (pointer < SIZE_SEND_TRAMA))
		{
			buffer[pointer] = (unsigned char)*(valor + i);
			pointer++;
			i++;
		}
		else break;
	}
}

//FUNCIONES PARA CAPTURA DE DATOS EN ENTEROS.
static int parte[2];
static int parte2[2];

//FUNCIÓN PARA TRASMITIR LOS DATOS:
void trasmitirDatos(float (*getVolt) (int *), float (*getAmperios) (int *), short doed)
{
	if(doed)
	{
		//ENVIAR LOS DATOS:
		
		//ACTUALIZAR REGISTROS DE CONVERSIÓN.
		setCorUSART();
		//ACTUALIZAR REGISTRO DE CONVERSIÓN.
		setVoltUSART();
		
		//ACTUALIZAR VALOR DE TRASMICIÓN PARA CORRIENTE Y TENSIÓN:
		EnviarCadena();	
	}
	else
	{
		//ACTUALIZAR LOS VALORES:
		getAmperios(parte);
		getVolt(parte2);
	}
}

////////////////FUNCIONES DE TRANSFERENCIA DE CARACTERES///////////////////////

void EnviarCadena()
{
	for(int i=0; i < pointer;i++)
	{
		while(!(UCSR0A & (1 << TXC0)));
		UCSR0A |= (1 << TXC0);
		UDR0 = buffer[i];
	}
	if(stop) initTrasmit = 0;
	pointer = 0;
	PORTD ^= (1 << 3);
}

/////////////FIN FUNCIONES DE TRANSFERENCIA DE CARACTERES/////////////////////

//CONFIGURAR EL VALOR DEL VOLTAJE:
void setVoltUSART()
{
	char valores[8];
	sprintf(valores, "0,%d.%02d,", parte2[0], parte2[1]);
	asignar(valores);
}

//CONFIGURAR EL VALOR DE LA CORRIENTE.
void setCorUSART()
{ 
	char valores[8];
	sprintf(valores, "1,%d.%02d,", parte[0], parte[1]);
	asignar(valores);
}