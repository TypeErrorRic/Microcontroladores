/*
 * ConfigTimer.h
 *
 * Created: 12/05/2023 12:24:35 a. m.
 *  Author: ricardo
 */ 


#ifndef CONFIGTIMER_H_
#define CONFIGTIMER_H_

void initTimer();
void initLcd();
void mostrarmensajeLCD(int *contador, float (*volts)(int *), float (*Ampers)(int *));

#endif /* CONFIGTIMER_H_ */