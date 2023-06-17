/*
 * ConfigTimer.c
 *
 * Created: 12/05/2023 12:24:50 a. m.
 *  Author: ricardo
 */ 

#include <stdio.h>

#include "lcd.h"

void initTimer()
{
	TCCR2A = 0x00;
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
	TCNT2 = 0;
}

void initLcd()
{
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_home();
}

static int desbordamiento(int *contador)
{
	if(TIFR2 & (1 << TOV2))
	{
		TIFR2 |= (1 << TOV2);
		if((*contador) == 10)
			return 1;
		*contador = (*contador) + 1;
	}
	return 0;
}

//VARIABLE PARA ALMACENAR LA CADENA DE CARACTERES:
static char buffer[20];
//Variables para calculo de voltaje:
static int parte[2];
//Variables para calculo de corriente:
static int parte2[2];

void mostrarmensajeLCD(int *contador, float (*volts)(int *), float (*Ampers)(int *))
{
	if(desbordamiento(contador))
	{
		lcd_clrscr();
		lcd_home();
		lcd_gotoxy(0,0);
		//Voltaje:
		volts(parte);
		sprintf(buffer, "Volt: %d.%02d V", parte[0], parte[1]);
		lcd_puts(buffer);
		
		//Corriente:
		lcd_gotoxy(0,1);
		Ampers(parte2);
		sprintf(buffer, "Cor: %d.%02d A", parte2[0], parte2[1]);
		lcd_puts(buffer);
		contador = 0;
		PORTD ^= (1 << 2);	
	}
}