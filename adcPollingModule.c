/*
 * adcPollingModule.c
 *
 * Created: 2019-07-02 09:22:48 a.m.
 *  Author: Grupo USART.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

// ADC configuration
void configADC()
{
	//CONFIGURACIÓN DEL ADC:
	//PRESCALER: 64.
	//INTERCONEXIÓN ENTRE EL AVCC Y AREF.
	//PUERTO DE ENTRADA: A0 y A3.
	//MODO DE OPERACIÓN: Single.
	
	ADCSRA |= (1 << ADPS1) | (1 << ADPS2);
	ADMUX |= (1 << REFS0);
	//ADC Auto Trigger Enable:
	ADCSRA &= ~(1 << ADATE);
	//MODO DE OPERACIÓN:
	
	// Enables ADC for use
	ADCSRA |= (1 << ADEN);
}

//INICIAR LA CONVERSIÓN DEL ADC: 
void startADC()
{
	ADCSRA |= (1 << ADSC);			// Start ADC
}

//VERIFICAR SI YA TERMINO LA CONVERSIÓN:
int isADCFinish()
{
	return (ADCSRA & (1<<ADIF));
}

//OBTENER EL DATO:
unsigned int getADCData()
{
	ADCSRA |= (1<<ADIF); //LIMPIAR BANDERA.
	return ADCW;
}

// Change ADC channel
void changeADCchannel(unsigned char adcCH)
{
	ADMUX &= ~((1<<MUX0) | (1<<MUX1));
	if (adcCH == 1)
		ADMUX |= (1<<MUX1) | (1<<MUX0);
}
