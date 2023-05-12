/*
 * Proyecto de Microcontrolador.c
 *
 * Created: 8/05/2023 6:55:55 p. m.
 * Author : ricardo
 */ 

#include <avr/io.h>
#include "lcd.h"
#include "adcPollingModule.h"

#define F_CPU	8000000UL

#include <util/delay.h>

#include <stdio.h>

#define sensibilidad  0.100;

//VARIABLES DEL PROGRAMA:

//Captura los datos de voltaje y corriente para mostrar por la LCD:
char buffer[20];
char buffer2[20];
//Valores:
float valores[2];
float voltaje = 0;
float corriente = 0;
//Variables para calculo de voltaje:
int parte_entera = 0;
int parte_decimal = 0;
//Variables para calculo de corriente:
int parte_entera2 = 0;
int parte_decimal2 = 0;

unsigned char adcChannel = 0x00;

int main(void)
{	
	DDRC = 0x00;
	
	//Inits
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_home();

	//Init adc:
	configADC();
	
	while(1)
	{
	
		lcd_clrscr();
		lcd_home();
		
		for (adcChannel=0; adcChannel<2; adcChannel++)
		{
			changeADCchannel((unsigned char)adcChannel);
			startADC();
			while(!isADCFinish());					
			valores[adcChannel] = getADCData();
		}
		
		//CONVERSIÓN VOLTAJE:
		voltaje = (float)120*valores[0]/1023;
		
		parte_entera = (int)voltaje;
		parte_decimal = (int)((voltaje - parte_entera) * 100);
		
		//Corriente:
		corriente = (float)5*valores[1]/1023;
		corriente = (float)(corriente - 2.5)/sensibilidad;
		
		parte_entera2 = (int)corriente;
		if(parte_entera2 > corriente)
			parte_decimal2 = (int)((-1)*(corriente - parte_entera2) * 100);
		else 
			parte_decimal2 = (int)((corriente - parte_entera2) * 100);
		
		//MOSTRAR MENSAJES POR LA LCD:
		
		lcd_gotoxy(0,0);
		//Voltaje:
		sprintf(buffer, "Volt: %d.%02d V", parte_entera, parte_decimal);
		lcd_puts(buffer);
		
		lcd_gotoxy(0,1);
		sprintf(buffer2, "Cor: %d.%02d A", parte_entera2, parte_decimal2);
		lcd_puts(buffer2);
		//TIEMPO MUERTO:
		_delay_ms(100);
	}
}

