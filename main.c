/*
 * Proyecto de Microcontrolador.c
 *
 * Created: 8/05/2023 6:55:55 p. m.
 * Author : ricardo
 */ 

#include <avr/io.h>

#define F_CPU	16000000UL

#include "adcPollingModule.h"
#include "ConfigTimer.h"
#include "Usart_TX.h"

//VARIABLES DEL PROGRAMA:
//VARIABLES DE REGULACIÓN.
int contador = 0;

int main(void)
{	
	DDRC = 0x00;
	DDRD = 0xfe;
	
	//INIT LCD:
	initLcd();

	//INIT ADC:
	configADC();
	
	//INIT TIMER:
	initTimer();
	
	//INIT USART:
	//VELOCIDAD DE ENVIO DE CARACTERES EN MILISEGUNDOS.
	doUSARTinit(1000);
	
	while(1)
	{	
		for (unsigned char adcChannel=0; adcChannel<2; adcChannel++)
		{
			changeADCchannel(adcChannel);
			startADC();
			while(!isADCFinish());					
			getADCData(adcChannel);
		}
		
		//ENVIAR DATOS POR USART:
		trasmitirDatos(getVolt, getAmperios, 0);
		
		//MOSTRAR MENSAJES POR LA LCD:
		mostrarmensajeLCD(&contador, getVolt, getAmperios);
	}
}

