/*
 * adcPolling.h
 *
 * Created: 2018-09-24 09:38:54 a.m.
 *  Author: BladimirBaccaCortes
 */ 

/* Code for single pin addressing */

#ifndef ADCPOLLING_H
#define ADCPOLLING_H

#define sensibilidad	((float) 0.066)
#define NUM_MUESTRA		500

// Functions.
void configADC();
void startADC();
int isADCFinish();
void getADCData(unsigned char value);
void changeADCchannel(unsigned char adcCH);
float getVolt(int *parte);
float getAmperios(int *parte);

#endif