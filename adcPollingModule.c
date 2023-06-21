/*
 * adcPollingModule.c
 *
 * Created: 2019-07-02 09:22:48 a.m.
 *  Author: Grupo USART.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>

#include "adcPollingModule.h"

// ADC configuration
void configADC()
{
	//CONFIGURACIÓN DEL ADC:
	//PRESCALER: 2.
	//INTERCONEXIÓN ENTRE EL AVCC Y AREF.
	//PUERTO DE ENTRADA: A0 y A3.
	//MODO DE OPERACIÓN: Single.
	ADCSRA = 0x00;
	ADMUX |= (1 << REFS0);
	//ADC Auto Trigger Enable:
	ADCSRA &= ~(1 << ADATE);
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

//////////////////////////////FUNCIONAMIENTO DEL SISTEMA/////////////////////////

//VALOR MEDIO DE LA CORRIENTE:
static float corriente = 0;
static double corrTrueRms = 0;
static float corrMin = 0;
static float corrMax = 0;
//VALOR MEDIO DEL VOLATAJE:
static float voltaje = 0;
static double voltTrueRms = 0;
static float voltMax = 0;
static float voltMin = 0;
//CONTADOR DE CUENTAS:
static unsigned int contador = 0;
static unsigned int contadorV = 0;

void getADCData(unsigned char value)
{
	ADCSRA |= (1<<ADIF); //LIMPIAR BANDERA.
	switch(value)
	{
		case 1:
		//CORRIENTE CALCULO:
		if(contador < NUM_MUESTRA)
		{
			float corr = (float) 5 * ((int) ADCW) / 1023;
			corr = (float) (corr - 2.5) / sensibilidad;
			corrTrueRms += pow(corr, 2);
			if(corr> corrMax)
				corrMax = corr;
			else if (corr < voltMin)
				corrMin = corr;
			contador++;
		}
		else
		{
			corriente = (sqrt(corrTrueRms / contador) * FILTROA) + ((((corrMax - corrMin) / 2) * 0.707) * (1 - FILTROA));
			corrTrueRms = 0;
			//REINICIO DE VARIABLES:
			corrMin = 0;
			corrMax = 0;
			contador = 0;
		}
		break;
		case 0:
		//CALCULO VOLTAJE:
		if(contadorV < NUM_MUESTRA)
		{
			float volt = (float) 110 * ((int) ADCW) / 1023;
			voltTrueRms += pow(((volt - 55) / REDUCCION), 2);
			if(volt > voltMax)
				voltMax = volt;
			else if (volt < voltMin)
				voltMin = volt;
			contadorV++;
		}
		else
		{
			voltaje =((sqrt(voltTrueRms / contadorV) * REDUCCION) * FILTROV) + ((((voltMax - voltMin) / 2) * 0.707) * (1 - FILTROV));
			voltTrueRms = 0;
			//REINICIO DE VARIABLES:
			voltMax = 0;
			voltMin = 0;
			contadorV = 0;
		}
		break;
	}
}

// Change ADC channel
void changeADCchannel(unsigned char adcCH)
{
	ADMUX &= ~((1<<MUX0) | (1<<MUX1)); // ADC0.
	if (adcCH == 1)
	{
		ADMUX |= (1<<MUX1) | (1<<MUX0); //ADC3.
	}
}

//CÓDIGO PARA RETORNAR LA TENSION DE LA LINEA DE ALIMENTACIÓN:
float getVolt(int *parte)
{
	if (parte != NULL)
	{
		*(parte + 0) = (int)voltaje;
		*(parte + 1) = (int)((voltaje - *(parte + 0)) * 100);
	}
	return voltaje;
}

//CÓDIGO PARA RETORNAR LA CORRIENTE DE LA LINEA DE ALIMENTACIÓN.
float getAmperios(int *parte)
{
	if(parte != NULL)
	{
		//CONVERSION DE TIPO DE DATO.
		*(parte + 0) = (int)corriente;
		if(*(parte + 0) > corriente)
		*(parte + 1) = (int)((-1)*(corriente - *(parte + 0)) * 100);
		else
		*(parte + 1) = (int)((corriente - *(parte + 0)) * 100);
	}
	return corriente;	
}
