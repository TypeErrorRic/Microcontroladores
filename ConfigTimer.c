/*
 * ConfigTimer.c
 *
 * Created: 12/05/2023 12:24:50 a. m.
 *  Author: ricardo
 */ 

#include <avr/io.h>

void initTimer()
{
	TCCR0A = 0x00;
	TCCR0B |= (1 << CS02) | (1 << CS00);
	TCNT0 = 0;
}

int desbordamiento(int *contador)
{
	if(TIFR0 & (1 << TOV0))
	{
		TIFR0 |= (1 << TOV0);
		if((*contador) == 10)
			return 1;
		*contador = (*contador) + 1;
	}
	return 0;
}