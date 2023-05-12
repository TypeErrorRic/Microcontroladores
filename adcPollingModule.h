/*
 * adcPolling.h
 *
 * Created: 2018-09-24 09:38:54 a.m.
 *  Author: BladimirBaccaCortes
 */ 

/* Code for single pin addressing */

#ifndef ADCPOLLING_H
#define ADCPOLLING_H

// Functions.
void configADC();
void startADC();
int isADCFinish();
unsigned int getADCData();
void changeADCchannel(unsigned char adcCH);

#endif